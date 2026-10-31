#!/usr/bin/env python3
"""Local GUI bridge for the existing whole_project_demo C++ executable.
No third-party Python packages; binds to 127.0.0.1 only.
"""
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import json, os, re, subprocess, sys, webbrowser

HERE=Path(__file__).resolve().parent
ROOT=HERE.parent
BINARY=ROOT/'whole_project_demo'
ALLOWED={'dfs':'1','bfs':'2','ready':'3','reports':'4','scenario-tests':'5','integration':'6'}
INDUSTRIES={'healthcare':'1','logistics':'2'}

def run_demo(industry, action):
    if not BINARY.is_file():
        return {'ok':False,'output':'Executable missing. In your project root run: make -f Makefile.integration','exitCode':None}
    args=[str(BINARY)]
    if action=='all-tests':
        args+=['--test']
        input_data=None
    else:
        input_data=f"{INDUSTRIES[industry]}\n{ALLOWED[action]}\n0\n0\n"
    try:
        completed=subprocess.run(args,input=input_data,text=True,capture_output=True,timeout=25,cwd=ROOT)
    except subprocess.TimeoutExpired:
        return {'ok':False,'output':'The C++ program exceeded the 25-second timeout.','exitCode':None}
    except OSError as err:
        return {'ok':False,'output':str(err),'exitCode':None}
    out=(completed.stdout+'\n'+completed.stderr).strip()
    # Strip repetitive menu headers so the GUI focuses on actual engine output.
    if action!='all-tests':
        heading=re.search(r'(?m)^={3,} (?:Healthcare|Logistics|TEST:|INTEGRATION:)',out)
        if heading:
            out=out[heading.start():]
            menu=re.search(r'\n(?:Healthcare - patient admission|Logistics - order fulfilment)\n1\. Show depth-first traversal',out)
            if menu: out=out[:menu.start()]
    checks=re.findall(r'^\[(PASS|FAIL)\]\s+(.+)$',out,re.MULTILINE)
    nodes=[]
    for line in out.splitlines():
        match=re.match(r'^\s+([HL]-[A-Z-]+)\s+(Stage|SubProcess|Task)\s+(.+?)(?:\s+\[([A-Za-z]+)\])?\s*$',line)
        if match: nodes.append({'id':match[1],'type':match[2],'name':match[3], 'state':match[4] or ''})
    progress=re.search(r'Progress\s*\(completed tasks\)\s*:\s*([0-9]+(?:\.[0-9]+)?)\s*%',out,re.I)
    duration=re.search(r'Average\s+SAMPLE\s+task\s+duration\s*:\s*([0-9]+(?:\.[0-9]+)?)',out,re.I)
    total=re.search(r'TOTAL:\s*(\d+) passed,\s*(\d+) failed',out)
    return {'ok':completed.returncode==0,'output':out,'exitCode':completed.returncode,
            'nodes':nodes,'checks':[{'status':s,'description':d} for s,d in checks],
            'passed':len([1 for s,d in checks if s=='PASS']) if not total else int(total[1]),
            'failed':len([1 for s,d in checks if s=='FAIL']) if not total else int(total[2]),
            'progress':float(progress[1]) if progress else None,
            'sampleDuration':float(duration[1]) if duration else None,
            'reportParsed':bool(progress and duration) if action=='reports' else None}

class Handler(SimpleHTTPRequestHandler):
    def __init__(self,*args,**kwargs): super().__init__(*args,directory=str(HERE),**kwargs)
    def do_POST(self):
        if self.path!='/api/run': self.send_error(404);return
        try:
            n=int(self.headers.get('Content-Length','0'))
            if n>4000: raise ValueError('Request too large')
            data=json.loads(self.rfile.read(n).decode('utf-8'))
            industry=data.get('industry');action=data.get('action')
            if action!='all-tests' and (industry not in INDUSTRIES or action not in ALLOWED): raise ValueError('Invalid industry or action')
            result=run_demo(industry,action)
            encoded=json.dumps(result).encode()
            self.send_response(200);self.send_header('Content-Type','application/json; charset=utf-8')
            self.send_header('Cache-Control','no-store');self.send_header('Content-Length',str(len(encoded)))
            self.end_headers();self.wfile.write(encoded)
        except (ValueError,KeyError,json.JSONDecodeError) as err:
            self.send_error(400,str(err))
    def log_message(self,fmt,*args): print('[GUI] '+fmt%args)

if __name__=='__main__':
    port=int(os.environ.get('TIVIDY_GUI_PORT','8765'))
    print(f'TIVIDY GUI running at http://127.0.0.1:{port}',flush=True)
    if not BINARY.exists():print('NOTE: run make -f Makefile.integration in the project root first.',flush=True)
    ThreadingHTTPServer(('127.0.0.1',port),Handler).serve_forever()
