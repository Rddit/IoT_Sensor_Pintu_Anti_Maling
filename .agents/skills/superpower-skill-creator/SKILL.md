---
name: superpower-skill-creator
description: >-
  Use this skill to design, scaffold, validate, and create high-quality custom skills,
  rules, and automated workflows for Antigravity agents following official customization standards.
---

# Superpower Skill Creator

A meta-skill that empowers the agent to architect, scaffold, and generate fully compliant, high-performing skills for any project or workflow.

## Skill Creation Workflow

When tasked with creating a new skill:

1. **Analyze Requirements & Intent**:
   - Determine what domain or capability needs to be packaged into a skill.
   - Establish whether the procedure requires executable helper scripts, templates, or markdown guides.

2. **Define Directory Structure**:
   Ensure each skill lives in `.agents/skills/<skill-name>/`:
   ```text
   .agents/skills/<skill-name>/
   ├── SKILL.md          # Required: YAML frontmatter + operational runbook
   ├── scripts/          # Optional: Executable automation scripts
   ├── examples/         # Optional: Sample configurations or boilerplate
   └── references/       # Optional: In-depth technical docs (progressive disclosure)
   ```

3. **Craft the Frontmatter**:
   The frontmatter must adhere strictly to YAML format:
   ```markdown
   ---
   name: <lowercase-kebab-case>
   description: >-
     A concise, third-person description explaining WHAT the skill does and
     WHEN the agent should activate it.
   ---
   ```

4. **Structure the `SKILL.md` Body**:
   - **Overview**: Clear problem statement and goal.
   - **Prerequisites & Tooling**: Tools, libraries, or dependencies needed.
   - **Step-by-Step Procedure**: Actionable, deterministic steps.
   - **Verification & Testing**: Clear acceptance criteria to confirm success.
   - **Edge Cases & Failure Recovery**: Troubleshooting tips for typical traps.

5. **Apply Progressive Disclosure**:
   - Keep `SKILL.md` focused on actionable instructions (< 300 lines).
   - Place large datasets, datasheets, or code templates in `references/` or `examples/`.
