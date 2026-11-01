"""Local SQLite workflow workspace; independent of the C++ demonstration engine."""
import json
import os
import sqlite3
import uuid
from datetime import datetime, timezone
from pathlib import Path

DB = Path(os.environ.get('TIVIDY_DB_PATH', str(Path(__file__).resolve().parent / 'workflow_data.sqlite3')))
STATES = ('Created', 'Available', 'Assigned', 'InProgress', 'OnHold', 'Completed', 'Cancelled')
BUILTINS = {
    'healthcare': ('Healthcare · Patient admission', 'Healthcare', 'Register, assess, treat and discharge a patient',
                   [('Intake', ['Register patient', 'Assess patient']), ('Care and discharge', ['Provide treatment', 'Prepare discharge'])]),
    'logistics': ('Logistics · Order fulfilment', 'Logistics', 'Receive, verify, pick and dispatch an order',
                  [('Order receiving', ['Capture order', 'Verify stock']), ('Picking and dispatch', ['Pick and pack', 'Dispatch delivery'])]),
}

def now():
    return datetime.now(timezone.utc).isoformat(timespec='seconds')

def ident():
    return uuid.uuid4().hex[:12]

def connect():
    DB.parent.mkdir(parents=True, exist_ok=True)
    c = sqlite3.connect(DB, timeout=15)
    c.row_factory = sqlite3.Row
    c.execute('PRAGMA foreign_keys=ON')
    c.execute('PRAGMA busy_timeout=15000')
    return c

def init():
    with connect() as c:
        c.executescript('''
        CREATE TABLE IF NOT EXISTS scenarios (id TEXT PRIMARY KEY, name TEXT NOT NULL, industry TEXT NOT NULL, description TEXT NOT NULL DEFAULT '', created_at TEXT NOT NULL, builtin_key TEXT NOT NULL DEFAULT '');
        CREATE TABLE IF NOT EXISTS stages (id TEXT PRIMARY KEY, scenario_id TEXT NOT NULL REFERENCES scenarios(id) ON DELETE CASCADE, name TEXT NOT NULL, position INTEGER NOT NULL);
        CREATE TABLE IF NOT EXISTS tasks (id TEXT PRIMARY KEY, stage_id TEXT NOT NULL REFERENCES stages(id) ON DELETE CASCADE, name TEXT NOT NULL, description TEXT NOT NULL DEFAULT '', assignee TEXT NOT NULL DEFAULT '', role TEXT NOT NULL DEFAULT '', status TEXT NOT NULL DEFAULT 'Created', position INTEGER NOT NULL, created_at TEXT NOT NULL, started_at TEXT NOT NULL DEFAULT '', completed_at TEXT NOT NULL DEFAULT '', depends_on TEXT NOT NULL DEFAULT '[]');
        CREATE TABLE IF NOT EXISTS events (id INTEGER PRIMARY KEY AUTOINCREMENT, scenario_id TEXT NOT NULL REFERENCES scenarios(id) ON DELETE CASCADE, task_id TEXT, actor TEXT NOT NULL, action TEXT NOT NULL, detail TEXT NOT NULL, timestamp TEXT NOT NULL);
        CREATE TABLE IF NOT EXISTS checkpoints (id TEXT PRIMARY KEY, scenario_id TEXT NOT NULL REFERENCES scenarios(id) ON DELETE CASCADE, name TEXT NOT NULL, snapshot TEXT NOT NULL, created_at TEXT NOT NULL);
        ''')
        # Migrate databases created by previous versions, without losing user scenarios.
        schema = {r['name'] for r in c.execute('PRAGMA table_info(scenarios)')}
        if 'builtin_key' not in schema:
            c.execute("ALTER TABLE scenarios ADD COLUMN builtin_key TEXT NOT NULL DEFAULT ''")
        schema = {r['name'] for r in c.execute('PRAGMA table_info(tasks)')}
        if 'depends_on' not in schema:
            c.execute("ALTER TABLE tasks ADD COLUMN depends_on TEXT NOT NULL DEFAULT '[]'")
        for key, (name, industry, desc, stages) in BUILTINS.items():
            if c.execute('SELECT 1 FROM scenarios WHERE builtin_key=?', (key,)).fetchone():
                continue
            # Identify older seeded scenarios by their exact name, preserving edits.
            old = c.execute('SELECT id FROM scenarios WHERE name=? AND industry=? AND builtin_key=\'\' ORDER BY created_at LIMIT 1', (name, industry)).fetchone()
            if old:
                c.execute('UPDATE scenarios SET builtin_key=? WHERE id=?', (key, old['id']))
            else:
                seed(c, name, industry, desc, stages, key)
        c.execute('CREATE UNIQUE INDEX IF NOT EXISTS builtin_unique ON scenarios(builtin_key) WHERE builtin_key != \'\'')

def event(c, sid, tid, action, detail, actor='Operator'):
    c.execute('INSERT INTO events(scenario_id,task_id,actor,action,detail,timestamp) VALUES(?,?,?,?,?,?)', (sid,tid,actor,action,detail,now()))

def seed(c, name, industry, description, stage_defs, builtin_key=''):
    sid = ident()
    c.execute('INSERT INTO scenarios(id,name,industry,description,created_at,builtin_key) VALUES(?,?,?,?,?,?)', (sid,name,industry,description,now(),builtin_key))
    previous = ''
    for ix,(stage_name,names) in enumerate(stage_defs):
        stage_id = ident()
        c.execute('INSERT INTO stages VALUES(?,?,?,?)', (stage_id,sid,stage_name,ix))
        for j,task_name in enumerate(names):
            tid = ident()
            c.execute('INSERT INTO tasks(id,stage_id,name,position,created_at,depends_on) VALUES(?,?,?,?,?,?)', (tid,stage_id,task_name,j,now(),json.dumps([previous] if previous else [])))
            previous = tid
    event(c,sid,None,'SCENARIO_CREATED',name)
    return sid

def full(c,sid):
    row = c.execute('SELECT * FROM scenarios WHERE id=?', (sid,)).fetchone()
    if row is None: raise ValueError('Scenario not found')
    result=dict(row);result['stages']=[]
    for stage in c.execute('SELECT * FROM stages WHERE scenario_id=? ORDER BY position,id',(sid,)):
        data=dict(stage)
        data['tasks']=[]
        for t in c.execute('SELECT * FROM tasks WHERE stage_id=? ORDER BY position,id',(stage['id'],)):
            item=dict(t);item['depends_on']=json.loads(item['depends_on'])
            data['tasks'].append(item)
        result['stages'].append(data)
    return result

def list_scenarios():
    with connect() as c:
        rows=c.execute("SELECT id FROM scenarios ORDER BY CASE WHEN builtin_key='healthcare' THEN 0 WHEN builtin_key='logistics' THEN 1 ELSE 2 END, created_at, id").fetchall()
        return [full(c,r['id']) for r in rows]

def text(data,key,limit=160,required=True):
    value=data.get(key,'')
    if not isinstance(value,str): raise ValueError(f'{key} must be text')
    value=value.strip()
    if len(value)>limit or (required and not value): raise ValueError(f'{key} must contain {1 if required else 0}–{limit} characters')
    return value

def belongs(c,task_id,sid):
    return c.execute('SELECT t.* FROM tasks t JOIN stages st ON t.stage_id=st.id WHERE t.id=? AND st.scenario_id=?',(task_id,sid)).fetchone()

def perform(data):
    if not isinstance(data,dict): raise ValueError('Expected a JSON object')
    action=data.get('action')
    with connect() as c:
        if action=='create_scenario':
            sid=seed(c,text(data,'name'),text(data,'industry',60),text(data,'description',500,False),[])
            return full(c,sid)
        if action=='add_stage':
            sid=text(data,'scenario_id');full(c,sid)
            name=text(data,'name')
            pos=c.execute('SELECT COUNT(*) FROM stages WHERE scenario_id=?',(sid,)).fetchone()[0]
            c.execute('INSERT INTO stages VALUES(?,?,?,?)',(ident(),sid,name,pos))
            event(c,sid,None,'STAGE_ADDED',name)
            return full(c,sid)
        if action=='add_task':
            stage_id=text(data,'stage_id');name=text(data,'name')
            stage=c.execute('SELECT scenario_id FROM stages WHERE id=?',(stage_id,)).fetchone()
            if stage is None: raise ValueError('Stage not found')
            sid=stage['scenario_id']
            dep=text(data,'depends_on',30,False)
            if dep and not belongs(c,dep,sid): raise ValueError('Prerequisite must belong to this scenario')
            pos=c.execute('SELECT COUNT(*) FROM tasks WHERE stage_id=?',(stage_id,)).fetchone()[0]
            c.execute('INSERT INTO tasks(id,stage_id,name,description,role,position,created_at,depends_on) VALUES(?,?,?,?,?,?,?,?)',(ident(),stage_id,name,text(data,'description',500,False),text(data,'role',80,False),pos,now(),json.dumps([dep] if dep else [])))
            event(c,sid,None,'TASK_ADDED',name)
            return full(c,sid)
        if action=='delete_task':
            tid=text(data,'task_id')
            row=c.execute('''SELECT t.*,st.scenario_id FROM tasks t
                             JOIN stages st ON st.id=t.stage_id
                             WHERE t.id=?''',(tid,)).fetchone()
            if row is None: raise ValueError('Task not found')
            sid=row['scenario_id']
            # Check dependencies across *every stage* in the scenario.
            other_tasks=c.execute('''SELECT t.id,t.name,t.depends_on FROM tasks t
                                     JOIN stages st ON st.id=t.stage_id
                                     WHERE st.scenario_id=? AND t.id!=?''',(sid,tid)).fetchall()
            dependents=[t['name'] for t in other_tasks if tid in json.loads(t['depends_on'])]
            if dependents:
                raise ValueError('Cannot delete this task: required by '+', '.join(dependents)+'. Remove or change those dependencies first.')
            # Snapshot recovery records can refer to this task. Remove deleted task
            # from snapshots to keep future recovery from reviving stale state.
            for cp in c.execute('SELECT id,snapshot FROM checkpoints WHERE scenario_id=?',(sid,)).fetchall():
                snapshot=json.loads(cp['snapshot'])
                updated=[task for task in snapshot if task['id']!=tid]
                if len(updated)!=len(snapshot):
                    c.execute('UPDATE checkpoints SET snapshot=? WHERE id=?',(json.dumps(updated),cp['id']))
            c.execute('DELETE FROM tasks WHERE id=?',(tid,))
            event(c,sid,tid,'TASK_DELETED',row['name'])
            return full(c,sid)
        if action=='task_action':
            tid=text(data,'task_id');op=text(data,'operation')
            row=c.execute('SELECT t.*,st.scenario_id FROM tasks t JOIN stages st ON st.id=t.stage_id WHERE t.id=?',(tid,)).fetchone()
            if row is None: raise ValueError('Task not found')
            state=row['status'];new=None;fields={};detail=op
            if op=='make_available' and state=='Created':
                for dep in json.loads(row['depends_on']):
                    predecessor=belongs(c,dep,row['scenario_id'])
                    if predecessor is None or predecessor['status']!='Completed':
                        raise ValueError('Prerequisite must be completed before making this task available')
                new='Available'
            elif op=='assign' and state=='Available':
                assignee=text(data,'assignee',80)
                new='Assigned';fields['assignee']=assignee;detail='Assigned to '+assignee
            elif op=='start' and state=='Assigned': new='InProgress';fields['started_at']=now()
            elif op=='hold' and state=='InProgress':new='OnHold'
            elif op=='resume' and state=='OnHold':new='InProgress'
            elif op=='complete' and state=='InProgress':new='Completed';fields['completed_at']=now()
            elif op=='cancel' and state in STATES[:5]:new='Cancelled'
            else:raise ValueError(f'Invalid transition: {state} → {op}')
            fields['status']=new
            c.execute('UPDATE tasks SET '+','.join(k+'=?' for k in fields)+' WHERE id=?',(*fields.values(),tid))
            event(c,row['scenario_id'],tid,op.upper(),f'{row["name"]}: {state} → {new}. {detail}')
            return full(c,row['scenario_id'])
        if action=='checkpoint':
            sid=text(data,'scenario_id');name=text(data,'name',80);s=full(c,sid)
            snap=[dict(t) for stage in s['stages'] for t in stage['tasks']]
            cid=ident();c.execute('INSERT INTO checkpoints VALUES(?,?,?,?,?)',(cid,sid,name,json.dumps(snap),now()))
            event(c,sid,None,'CHECKPOINT',name)
            return {'id':cid,'scenario_id':sid}
        if action=='restore':
            cid=text(data,'checkpoint_id')
            ch=c.execute('SELECT * FROM checkpoints WHERE id=?',(cid,)).fetchone()
            if ch is None:raise ValueError('Checkpoint not found')
            snap=json.loads(ch['snapshot'])
            # Restore only existing tasks from snapshot; tasks added later stay unchanged.
            for task in snap:
                c.execute('UPDATE tasks SET status=?,assignee=?,started_at=?,completed_at=? WHERE id=? AND stage_id IN (SELECT id FROM stages WHERE scenario_id=?)',(task['status'],task['assignee'],task['started_at'],task['completed_at'],task['id'],ch['scenario_id']))
            event(c,ch['scenario_id'],None,'RESTORE',ch['name'])
            return full(c,ch['scenario_id'])
        if action=='reset_scenario':
            sid=text(data,'scenario_id');full(c,sid)
            c.execute("UPDATE tasks SET status='Created',assignee='',started_at='',completed_at='' WHERE stage_id IN (SELECT id FROM stages WHERE scenario_id=?)",(sid,))
            event(c,sid,None,'RESET','All task execution states reset to Created')
            return full(c,sid)
        if action=='delete_scenario':
            sid=text(data,'scenario_id');s=full(c,sid)
            if s['builtin_key']: raise ValueError('Built-in scenarios cannot be deleted. Use Reset instead.')
            c.execute('DELETE FROM scenarios WHERE id=?',(sid,))
            return {'deleted':True}
        raise ValueError('Unknown operation')

def events(sid):
    with connect() as c:
        full(c,sid)
        return [dict(x) for x in c.execute('SELECT * FROM events WHERE scenario_id=? ORDER BY id DESC LIMIT 200',(sid,))]

def checkpoints(sid):
    with connect() as c:
        full(c,sid)
        return [dict(x) for x in c.execute('SELECT id,name,created_at FROM checkpoints WHERE scenario_id=? ORDER BY created_at DESC,id DESC',(sid,))]
