# TIVIDY — Generic Workflow Management Engine

**COS 214 · Project · University of Pretoria**  
**Language:** C++11 · **Interface:** Local web GUI (Python, HTML, CSS, JavaScript)

## Overview

TIVIDY is a generic, workflow-management system that can be reused to represent and demonstrate business processes in various industry sectors. Instead of hard coding processes to one sector, the C++ engine describes all of the above: processes/workflow definitions, independent processes/workflow instances, stages, subprocesses, tasks, commands, and state changes.

The repository illustrates the following fundamental concepts with respect to workflow in two sample industries:

- Healthcare — patient admission: register patient, assess patient, provide treatment to patient, prepare patient for discharge.
- Logistics — order fulfilment: create an order, check inventory, pick and pack inventory and send a shipment.

The web interface supplied with the system offers a visual workspace (graphical representation) for creating and managing scenarios as well as a dedicated bridge to run the C++ demonstrations and integration checks.

## Key capabilities

- **Reusable workflow structures:** definitions made of stages, subprocesses and tasks, with separate instances for individual runs.
- **Task lifecycle and commands:** task assignment, starting, completion, and workflow actions.
- **Traversal and reporting:** depth-first, breadth-first, and ready-item traversal; progress, cycle-time and bottleneck reports.
- **Validation and testing:** checks for invalid workflow structures, state transitions and cross-module behaviour.
- **Assignments and recovery:** role/skill-based assignment components, command history, snapshots and recovery components.
- **Events and dependencies:** workflow event publishing and dependency-resolution components.
- **Local GUI workspace:** scenario and task management, task dependencies, event history and checkpoints backed by SQLite.

> **Architecture note:** The GUI's SQLite workspace is implemented separately from the C++ demonstration engine. The GUI server launches `whole_project_demo` for C++ demonstrations and tests. We must not assume that editing a GUI workspace task automatically mutates a live C++ `WorkflowInstance`.

## Repository layout

```text
.
├── TividyEngine.{h,cpp}          # Engine-level interface
├── WorkflowDefinition.{h,cpp}    # Process templates
├── WorkflowInstance.{h,cpp}      # Individual workflow runs
├── WorkNode.{h,cpp}              # Work-tree structure
├── Stage.{h,cpp} / SubProcess.{h,cpp}
├── WorkflowCommand.{h,cpp}       # Workflow command abstraction
├── CommandProcessor.{h,cpp}      # Command execution
├── DependencySet.{h,cpp}         # Dependency data
├── include/                      # Additional event, connector and resolver components
├── WholeProject_IntegrationMain.cpp  # Interactive C++ demo and integration tests
├── COS214_GUI/
│   ├── gui_server.py             # Local HTTP server and C++ bridge
│   ├── workflow_store.py         # SQLite-backed workspace
│   ├── index.html
│   ├── app.js
│   └── style.css
├── tests/                        # Unit and workspace tests
├── Makefile                      # Main build/run/test entry points
├── Makefile.integration          # C++ integration-demo build
└── Makefile.tests                # C++ unit-test build
```

## Prerequisites:

- A **C++11-compatible compiler** (`g++`).
- **GNU Make**.
- **Python 3** to launch the GUI. The GUI server uses Python's standard library, including `sqlite3` and no third-party Python packages are required for the server.
- A modern designed web browser for the local GUI interface.

**Windows:** With the appropriate GCC and Make packages installed, the project can be built from an **MSYS2 UCRT64** terminal . Run the commands below from the repository root to build the project. In PowerShell, `make` may not be on your PATH even if it works in MSYS2.

## Quick start

### 1. Build the C++ demonstration

```bash
make
```

The main Makefile delegates to `Makefile.integration` and produces `whole_project_demo`. 

### 2. Run the interactive demonstration

```bash
make run
```

Choose **Healthcare** or **Logistics** from the terminal menu, then traverse the system, construct reports and scenario checks. The demonstration uses descriptive and easy workflow data.

### 3. Run integration tests

```bash
make test
```

The integration test target runs:

```bash
./whole_project_demo --test
```

### 4. Launch the web GUI

```bash
make gui
```

Then open **http://127.0.0.1:8765** in your browser. Leave the terminal running while using the interface. The server will bind strictly to the local machine. If the port is already in use, close the other process.

The GUI stores workspace data in a  SQLite database (`COS214_GUI/workflow_data.sqlite3` by default). You can set `TIVIDY_DB_PATH` to use different database files.

## Additional testing

Run the separate C++ unit-test suite:

```bash
make test-unit
```

Run both unit and integration suites:

```bash
make test-all
```

Run the Python workspace tests (with Python available):

```bash
python3 -m unittest discover -s tests -p 'test_*.py'
```

**Test scope:** The default `make` target builds the *integration demonstration's chosen source set*. It does not build all the `.cpp` file in the repository. A successful `make` or `make test` does not, by itself, prove that all the independent module, files or possible build configurations compile and run.

## Design and architecture

TIVIDY's codebase includes is associated with several OO design patterns, including:

| Pattern / concept | Representative code | Purpose |

 Composite - `WorkNode`, `Stage`, `SubProcess` | Represent nested work structures. |
 Iterator -  `DepthFirstIterator`, `LevelOrderIterator`, `ReadyItemIterator` | Traverse or filter workflow nodes. |
 Visitor - `ProgressVisitor`, `BottleneckVisitor`, `CycleTimeVisitor` | Collect reports without embedding every calculation in nodes. |
 Command - `WorkflowCommand`, `CommandProcessor`, `AssignCommand`, `StartCommand`, `CompleteCommand` | Encapsulate workflow actions. |
 Strategy - `RoutingStrategy`, assignment strategy classes | Allow alternative routing and assignment policies. |
 Observer / events - `include/WorkflowEventPublisher.cpp`, event-related headers | Publish workflow events to interested components. |
 Dependency handling - `DependencySet`, `include/DependencyResolver.cpp` | Track and resolve prerequisites between work items. |

Other repository components cover factors such as approvals, escalation, snapshots, recovery, decorators, and connector abstractions. These are architectural components present in the codebase and they are  not all are exercised by the default demonstration target.

## Example demonstrations

**Healthcare:** Inspect a patient-admission work tree, compare differen traversal orders, observe task readiness and patient status changes, and view progress reports and duration reports.

**Logistics:** Inspect an order work tree, review all the stages from receiving orders through to dispatching different orders, and run the same traversal, validation and reporting operations without changing the underlying engine (ie structural changes).

**Reporting caveat:** The C++ demo labels all sample task times as illustrative test results, **not measured real-world processing times**.

## Troubleshooting

| Problem | What to check |
|---|---|
| `make: command not found` | Use an environment with GNU Make installed, such as MSYS2 UCRT64. |
| `g++: command not found` | Install a C++11 compiler and verify it is on the terminal PATH. |
| Missing header during compilation | Confirm you are using the repository Makefile and that `-I. -Iinclude` are present. |
| GUI says executable is missing | Run `make` in the repository root before `make gui`. |
| Port 8765 is occupied | Stop the other local server or set `TIVIDY_GUI_PORT`. |
| Unexpected data in the GUI | Check which SQLite database is selected through `TIVIDY_DB_PATH`. |

## Development workflow

Work on a feature branch, test locally, and open a pull request against the team's agreed integration branch. Avoid committing generated executables, build artifacts, local databases containing test data, or temporary files unless the team explicitly requires them.

## Project context

Developed as a team project for **COS 214 Practical 6** at the **University of Pretoria**. Consult the practical specification and the project's UML diagrams for the full assignment requirements and intended class relationships.

## Group Members

| Student Name | Student Number |
| -------- | -------- |
| Shelby Bodenstein | u25038967 |
| Hannah Diedrick | u25044215 |
| Kelly Rademeyer | u25571878 |
| Janke Rall | u24571238 |
| Livia Webber | u24607852 |
| Michael Harding | u25328086 |
| Tauhir September | u24750982 |

