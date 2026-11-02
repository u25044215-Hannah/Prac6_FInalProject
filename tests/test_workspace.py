"""Run: python3 -m unittest discover -s tests -v (from package root)."""
import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'COS214_GUI'))
import workflow_store as w

class WorkspaceTests(unittest.TestCase):
    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory()
        w.DB=Path(self.tmp.name)/'workflow.sqlite3'
        w.init()

    def tearDown(self):
        self.tmp.cleanup()

    def operate(self, action, **kw):
        return w.perform({'action':action, **kw})

    def test_two_builtins_on_every_launch(self):
        w.init(); w.init()
        scenarios=w.list_scenarios()
        self.assertEqual(len(scenarios),2)
        self.assertEqual({x['builtin_key'] for x in scenarios},{'healthcare','logistics'})
        self.assertEqual([sum(len(st['tasks']) for st in x['stages']) for x in scenarios],[4,4])
        for s in scenarios:
            self.assertEqual(len(s['stages']),2)
            ids=[t['id'] for st in s['stages'] for t in st['tasks']]
            deps=[t['depends_on'] for st in s['stages'] for t in st['tasks']]
            self.assertEqual(deps,[[],[ids[0]],[ids[1]],[ids[2]]])

    def test_create_assign_complete_report_persistence(self):
        s=self.operate('create_scenario',name='University admissions',industry='Education',description='Live demo')
        sid=s['id']
        s=self.operate('add_stage',scenario_id=sid,name='Applications')
        stage=s['stages'][0]['id']
        s=self.operate('add_task',stage_id=stage,name='Receive application',role='Clerk')
        t1=s['stages'][0]['tasks'][0]['id']
        s=self.operate('add_task',stage_id=stage,name='Verify documents',depends_on=t1)
        t2=s['stages'][0]['tasks'][1]['id']
        with self.assertRaisesRegex(ValueError,'Prerequisite'):
            self.operate('task_action',task_id=t2,operation='make_available')
        self.operate('task_action',task_id=t1,operation='make_available')
        self.operate('task_action',task_id=t1,operation='assign',assignee='Ada')
        self.operate('task_action',task_id=t1,operation='start')
        s=self.operate('task_action',task_id=t1,operation='complete')
        self.assertEqual(s['stages'][0]['tasks'][0]['status'],'Completed')
        self.operate('task_action',task_id=t2,operation='make_available')
        w.init()
        self.assertEqual(next(x for x in w.list_scenarios() if x['id']==sid)['stages'][0]['tasks'][1]['status'],'Available')
        self.assertTrue(any(x['action']=='COMPLETE' for x in w.events(sid)))

    def test_checkpoint_restore_reset_and_builtin_protection(self):
        s=w.list_scenarios()[0];sid=s['id']; tid=s['stages'][0]['tasks'][0]['id']
        cp=self.operate('checkpoint',scenario_id=sid,name='Initial')
        self.operate('task_action',task_id=tid,operation='make_available')
        s=self.operate('restore',checkpoint_id=cp['id'])
        self.assertEqual(s['stages'][0]['tasks'][0]['status'],'Created')
        self.operate('task_action',task_id=tid,operation='make_available')
        s=self.operate('reset_scenario',scenario_id=sid)
        self.assertEqual(s['stages'][0]['tasks'][0]['status'],'Created')
        with self.assertRaisesRegex(ValueError,'Built-in'):
            self.operate('delete_scenario',scenario_id=sid)

    def test_custom_delete_and_validation(self):
        s=self.operate('create_scenario',name='Custom',industry='Demo')
        sid=s['id']
        with self.assertRaises(ValueError):
            self.operate('add_task',stage_id='fake',name='A')
        self.operate('delete_scenario',scenario_id=sid)
        self.assertEqual(len(w.list_scenarios()),2)
        with self.assertRaises(ValueError):
            self.operate('add_stage',scenario_id=sid,name='A')

    def test_old_db_migrates(self):
        with w.connect() as c:
            c.execute('DROP TABLE tasks')
            c.execute('DROP TABLE scenarios')
            c.execute("CREATE TABLE scenarios(id TEXT PRIMARY KEY,name TEXT NOT NULL,industry TEXT NOT NULL,description TEXT NOT NULL DEFAULT '',created_at TEXT NOT NULL)")
            c.execute("CREATE TABLE tasks(id TEXT PRIMARY KEY,stage_id TEXT REFERENCES stages(id),name TEXT NOT NULL, description TEXT DEFAULT '', assignee TEXT DEFAULT '',role TEXT DEFAULT '',status TEXT DEFAULT 'Created', position INTEGER,created_at TEXT,started_at TEXT DEFAULT '',completed_at TEXT DEFAULT '')")
        w.init()
        self.assertEqual(len(w.list_scenarios()),2)

if __name__=='__main__':unittest.main()
