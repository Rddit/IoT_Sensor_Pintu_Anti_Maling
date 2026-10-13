---
name: project-context-keeper
description: >-
  Use this skill to track, persist, update, and execute the IoT Door Anti-Theft project's
  context, objectives, hardware pinouts, architectural decisions, and milestones.
---

# Project Context Keeper & Executor

Skill dedicated to managing, preserving, and systematically executing the objectives of the **IoT Door Anti-Theft Sensor (ESP32)** project.

## Core Responsibilities

1. **Context Persistence**:
   - Maintain the single source of truth in [`PROJECT_CONTEXT.md`](../../PROJECT_CONTEXT.md).
   - Ensure all hardware mappings (GPIO pinouts, logic levels, component revisions) remain synchronized across development sessions.
   - Maintain synchronization with editor workspace settings in [`.vscode/`](../../.vscode/) (`settings.json`, `arduino.json`, `tasks.json`, `c_cpp_properties.json`).
   - Prevent regression or conflicting wiring decisions.

2. **Goal & Milestone Execution**:
   - Check the progress checklist before starting any task.
   - Update task completion status in real-time.
   - Validate milestones against physical hardware constraints and compiler verification before marking them complete.

3. **Standard Context File Structure**:
   `PROJECT_CONTEXT.md` must maintain the following sections:
   - **Project Overview**: Core objective, target board, sensor types.
   - **Hardware Specifications & Pin Mapping**: Exact GPIO numbers, sensor state logic (HIGH/LOW), power levels.
   - **Software Architecture**: Firmware stack (Arduino C++), libraries, cloud platforms (Blynk / Telegram).
   - **Toolchain & Lingkungan Kerja**: Extensions, compiler versions, and verified builds.
   - **Active Status & Milestones**: Roadmap of what is DONE, IN PROGRESS, and PENDING.
   - **Decisions & Changelog**: History of technical choices (e.g. why `INPUT_PULLUP` was selected over external pull-up).

## Execution Runbook

Whenever the user asks a question or requests a modification:
1. Consult [`PROJECT_CONTEXT.md`](../../PROJECT_CONTEXT.md) first to ensure alignment.
2. If new hardware pins, libraries, or architectural directions are chosen, update [`PROJECT_CONTEXT.md`](../../PROJECT_CONTEXT.md) immediately.
3. Validate changes with the user when altering critical pinouts or credentials.
