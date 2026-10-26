# Core Engine and Workflow Instances

Module 1 of TIVIDY (Figure 5.1 of the class diagram) Branch: `core-engine`

## Purpose

This module is the generic centre of TIVIDY. It stores process templates, starts independent runs of them, and tracks how each run ends. It contains no industry knowledge, so the same code serves every scenario.

## Classes

| Class | Responsibility |
|---|---|
| `TividyEngine` | Single entry point used by the interface and the scenarios. Publishes definitions, starts workflows and submits commands. |
| `WorkflowDefinition` | The template. Holds a name, a version, the template work tree and the approval and escalation chains. Cannot be changed once published. |
| `DefinitionRepository` | Abstract storage for definitions, so the storage method can vary. |
| `InMemoryDefinitionRepository` | Default repository. Keeps definitions in memory and gives each new version of a process the next number. |
| `WorkflowInstance` | One run of a definition. Owns its own copy of the work tree, its data and its history. |
| `InstanceManager` | Creates instances by copying the definition's tree, then owns and finds them. |
| `VersionManager` | Chooses which definition version to run and applies a version policy when a new version is published. |
| `WorkflowData` | Plain text key/value information carried by an instance. The engine never interprets it. |
| `WorkflowOutcome` | The result of an instance: running, completed, rejected, cancelled or failed. |
| `TividyException` | Shared error types: not found and invalid operation. |

## How it fits together

1. A definition is built elsewhere and published through `TividyEngine`.
2. The repository stores it and assigns its version number.
3. Starting a workflow makes `InstanceManager` copy the definition's work tree into a new instance.
4. The instance runs on its own copy, so it never changes the template or other instances.
5. When it finishes, its outcome is recorded and its history stays available.

## Connections to other modules

| Module | What this module relies on |
|---|---|
| Work structure | `WorkNode`, with a deep copy operation and hooks for saving and restoring node states |
| Commands and recovery | Action history, commands, snapshots and the recovery manager |
| Rules and decisions | The migration strategy interface and the approval and escalation handlers |

## Author

Livia Ami Webber u24607852