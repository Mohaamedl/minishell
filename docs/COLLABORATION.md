# 🤝 Team Collaboration Guide

**Project**: Minishell  
**Team Size**: 2 members  
**Last Updated**: November 3, 2025  
**Jira Project**: KAN (mohaamedll.atlassian.net)

---

## 📋 Table of Contents

1. [Jira Workflow](#jira-workflow)
2. [Git Workflow](#git-workflow)
3. [Code Review Process](#code-review-process)
4. [Communication Guidelines](#communication-guidelines)
5. [Task Assignment Strategy](#task-assignment-strategy)
6. [Conflict Resolution](#conflict-resolution)
7. [Quality Standards](#quality-standards)

---

## 📊 Jira Workflow

### Project Structure

**Jira Project**: KAN    
**Total Tasks**: 59 tasks across 12 epics

### Epic Organization

Our project is organized into 12 epics covering the entire development lifecycle:

1. **KAN-4**: Project Setup & Foundation (ENV) - 7 tasks
2. **KAN-5**: Core Infrastructure (CORE) - Covered in foundation
3. **KAN-6**: Execution Engine (EXE) - 4 tasks
4. **KAN-7**: Bonus Features (BON) - 3 tasks
5. **KAN-8**: Redirections (RED) - 5 tasks
6. **KAN-9**: Pipes (PIP) - Integrated with redirections
7. **KAN-10**: Built-in Commands (BIN) - 7 tasks
8. **KAN-11**: Lexer (LEX) - 4 tasks
9. **KAN-12**: Parser (PAR) - 4 tasks
10. **KAN-13**: Expansion (EXP) - 3 tasks
11. **KAN-14**: Integration (INT) - 5 tasks
12. **KAN-15**: Testing (TST) - 8 tasks

### Task Workflow

**Jira Board Columns**:
```
To Do → In Progress → Code Review → Testing → Done
```

**Task Lifecycle**:
1. **To Do**: Task is ready to be picked up
2. **In Progress**: Actively working on implementation
3. **Code Review**: PR created, awaiting peer review
4. **Testing**: Code merged, undergoing testing
5. **Done**: Task complete, tested, and verified

### Sprint Planning

**Sprint Duration**: 1 week  
**Total Sprints**: 7-8 weeks

**Sprint Breakdown**:
- **Sprint 1 (Week 1)**: Foundation - KAN-16 to KAN-29
- **Sprint 2 (Week 2)**: Lexer & Token Structure - KAN-37 to KAN-40
- **Sprint 3 (Week 3)**: Parser & Built-ins - KAN-30 to KAN-44
- **Sprint 4 (Week 4)**: Expansion & Redirections - KAN-45 to KAN-51
- **Sprint 5 (Week 5)**: Pipes & Execution - KAN-52 to KAN-56
- **Sprint 6 (Week 6)**: Integration & Testing - KAN-57 to KAN-62
- **Sprint 7 (Week 7)**: Final Testing & Documentation - KAN-63 to KAN-71
- **Sprint 8 (Week 8)**: Bonus Features (Optional) - KAN-66 to KAN-68

### Daily Task Management

**Morning Routine**:
1. Check Jira board for assigned tasks
2. Review task dependencies
3. Update task status if needed
4. Comment on blockers

**During Work**:
1. Move task to "In Progress"
2. Add time tracking (optional)
3. Comment on progress/issues
4. Link commits to Jira tasks

**End of Day**:
1. Update task progress
2. Add comments on what's complete
3. Note any blockers for standup

### Linking Git Commits to Jira

Jira automatically tracks development activity when you reference work item keys in your Git workflow.

**Work Item Key Format**: `KAN-XX` (must use CAPITAL letters)

#### Branch Names
Include the work item key when creating branches:
```bash
# Correct format
git checkout -b KAN-23-implement-unset-env

# Alternative format
git checkout -b feature/KAN-23-unset-env-value
```

**Benefits**:
- Branch automatically linked to Jira task
- Shows in Development panel
- Track branch status on Jira board

#### Commit Messages
Include the work item key at the beginning of commit messages:
```bash
# Basic format
git commit -m "KAN-23 implement unset_env_value function"

# Detailed format (recommended)
git commit -m "KAN-23: feat(env): implement unset_env_value

- Remove environment variable from linked list
- Handle non-existent variables gracefully
- Add tests for edge cases"
```

**What happens**:
- Commits appear in task's Development panel
- Track code changes directly from Jira
- See commit history linked to each task

#### Pull Request Titles
Include the work item key in PR titles:
```bash
# PR Title format
KAN-23: Implement unset environment variable function

# Or with conventional commits
feat(env): KAN-23 - implement unset_env_value
```

**Auto-linking**:
- PR automatically linked to Jira task
- Status shows on Jira board card
- Click to view PR details from Jira

#### Complete Example Workflow
```bash
# 1. Find task key in Jira (e.g., KAN-23)

# 2. Create branch with key
git checkout -b KAN-23-unset-env-implementation

# 3. Make changes and commit with key
git add src/environment/unset.c
git commit -m "KAN-23: implement unset_env_value

Add function to remove environment variables
Handle edge cases for non-existent variables"

# 4. Push branch
git push origin KAN-23-unset-env-implementation

# 5. Create PR with key in title
# Title: "KAN-23: Implement unset_env_value function"

# 6. View in Jira
# - Navigate to KAN-23
# - See branch, commits, and PR in Development section
```

#### Viewing Development Info in Jira

**In Task View**:
1. Open task (e.g., KAN-23)
2. Look for "Development" section
3. Click to see:
   - Branches linked
   - Commits made
   - Pull requests
   - Build status (if configured)

**On Board View**:
- Development icons appear on cards
- Hover to see quick details
- Icons show: branches, PRs, commits

**Important Notes**:
- ✅ **Always use CAPITALS**: `KAN-23` not `kan-23`
- ✅ **Key at beginning**: Start commits with key
- ✅ **Push to sync**: Changes appear after pushing to GitHub
- ⏱️ **Sync delay**: May take 1-2 minutes for updates to show

### Task Assignment

**In Jira**:
1. Navigate to task
2. Click "Assign"
3. Select team member
4. Add to current sprint

**Balanced Distribution**:
- Member 1: ~30 tasks (focused on parsing, expansion, integration)
- Member 2: ~29 tasks (focused on lexer, execution, testing)
- Both: Pair programming on complex tasks

### Story Points & Velocity

**Story Point Scale** (Fibonacci):
- 1: Trivial (~1 hour)
- 2: Simple (~2 hours)
- 3: Easy (~3-4 hours)
- 5: Medium (~1 day)
- 8: Complex (~2 days)
- 13: Very Complex (~3 days)
- 21: Epic-sized (break down further)

**Target Velocity**: 30-40 story points per week per person

**Total Story Points**: ~250 points (mandatory features)

### Viewing Development Activity in Jira

#### On Individual Tasks

1. **Open any task** (e.g., KAN-23)
2. **Look for "Development" section** (right side panel)
3. **Click to expand** and see:
   - 🌿 **Branches**: All branches referencing this task
   - 📝 **Commits**: All commits with this task key
   - 🔀 **Pull Requests**: PRs linked to this task
   - ✅ **Build Status**: If CI/CD configured

**What you'll see**:
```
Development
├── 1 branch
│   └── KAN-23-unset-env-implementation
├── 3 commits
│   ├── KAN-23: implement unset function
│   ├── KAN-23: add edge case handling
│   └── KAN-23: add tests
└── 1 pull request
    └── #42: KAN-23 - Implement unset_env_value
```

#### On the Board

**Development Icons** appear on task cards:
- 🌿 Branch icon: Shows active branches
- 📝 Commit icon: Shows number of commits
- 🔀 PR icon: Shows pull request status
- 🚀 Deploy icon: Shows deployment status (if configured)

**How to use**:
1. Navigate to your Kanban board
2. **Hover over icons** on any card
3. See quick summary of development activity
4. **Click icon** to jump to GitHub/Git tool

**Note**: Icons only show when:
- At least one task has development data
- Board has less than 100 tasks

#### Development Activity Timeline

In each task, you can see:
- **When branches were created**
- **Commit history with messages**
- **PR creation and review status**
- **Merge activity**
- **Build/test results** (if configured)

This gives you full visibility into code progress without leaving Jira!

### Jira Reports

**Use for Tracking**:
1. **Burndown Chart**: Progress within sprint
2. **Velocity Chart**: Story points completed per sprint
3. **Cumulative Flow**: Work distribution across statuses
4. **Time Tracking**: Actual vs estimated time
5. **Development Activity**: Code commits and PRs per sprint

**Weekly Review Metrics**:
- Story points completed
- Tasks remaining
- Blockers identified
- Velocity trends
- **Development activity** (commits, PRs merged)
- **Code review turnaround time**

---

## 🌳 Git Workflow

### Branch Strategy

```
main (protected)
├── feature/environment-management
├── feature/signal-handling
├── feature/lexer
├── feature/parser
├── feature/builtins
├── feature/executor
└── feature/redirections
```

### Branch Naming Convention

**With Jira Integration** (Recommended):
- **Feature**: `KAN-XX-description` (e.g., `KAN-23-unset-env`)
- **Feature Alt**: `feature/KAN-XX-description` (e.g., `feature/KAN-23-env-management`)
- **Bug Fix**: `fix/KAN-XX-description` (e.g., `fix/KAN-45-memory-leak-parser`)
- **Documentation**: `docs/KAN-XX-description` (e.g., `docs/KAN-69-update-readme`)
- **Refactor**: `refactor/KAN-XX-description` (e.g., `refactor/KAN-26-clean-utils`)

**Without Jira Key** (Legacy):
- **Feature**: `feature/description`
- **Bug Fix**: `fix/description`
- **Documentation**: `docs/description`
- **Refactor**: `refactor/description`

**Important**: Always include the Jira key (KAN-XX) for automatic linking!

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Formatting (no code change)
- `refactor`: Code restructuring
- `test`: Adding tests
- `chore`: Maintenance tasks

**Examples with Jira Integration**:
```bash
# Feature implementation (start with Jira key)
KAN-19: feat(env): implement environment initialization

- Parse extern char **environ
- Create linked list of t_env nodes
- Handle memory allocation failures
- Add tests for empty environment

Related: Epic KAN-4 (Foundation)

---

# Bug fix (Jira key at start)
KAN-45: fix(parser): handle unclosed quotes correctly

Previously, parser would crash on unclosed quotes.
Now returns syntax error and continues.

Closes: KAN-45
Related: KAN-42 (Parser)

---

# Documentation update
KAN-69: docs(readme): update installation instructions

Add steps for readline installation on macOS

Related: Final documentation phase

---

# Alternative format (both work)
feat(env): KAN-19 - implement environment initialization
fix(parser): KAN-45 - handle unclosed quotes correctly
docs(readme): KAN-69 - update installation instructions
```

**Best Practices**:
- ✅ **Always start with Jira key**: `KAN-XX: type(scope): message`
- ✅ **Use capitals**: `KAN-23` not `kan-23`
- ✅ **Reference epic when relevant**: `Related: Epic KAN-4`
- ✅ **Link dependencies**: `Depends on: KAN-18`
- ✅ **Close tasks**: `Closes: KAN-XX` or `Fixes: KAN-XX`

### Daily Workflow

```bash
# 1. Start of day: Update your local main
git checkout main
git pull origin main

# 2. Check Jira for your assigned task (e.g., KAN-23)

# 3. Create feature branch with Jira key
git checkout -b KAN-23-unset-env-implementation
# OR
git checkout -b feature/KAN-23-unset-env

# 4. Update Jira: Move task to "In Progress"

# 5. Work on your feature (commit regularly with Jira key)
git add <files>
git commit -m "KAN-23: feat(env): implement unset_env_value

- Add unset function
- Handle edge cases
- Add tests"

# 6. Push to remote (Jira will auto-detect and link)
git push origin KAN-23-unset-env-implementation

# 7. Create Pull Request with Jira key in title
# Title: "KAN-23: Implement unset_env_value function"
# Description: Reference acceptance criteria from Jira task

# 8. Update Jira: Move task to "Code Review"

# 9. Wait for peer review

# 10. After approval:
# - Merge to main
# - Move Jira task to "Testing"
# - Delete feature branch
git branch -d KAN-23-unset-env-implementation
git push origin --delete KAN-23-unset-env-implementation

# 11. Verify feature works, then move task to "Done" in Jira
```

**Jira Integration Benefits**:
- ✅ All commits visible in Jira task
- ✅ Branch status shown on board
- ✅ PR linked automatically
- ✅ Full development history tracked

### Before Committing Checklist

- [ ] Code compiles without warnings
- [ ] Norminette passes
- [ ] No debug print statements
- [ ] Comments added where needed
- [ ] Tested manually
- [ ] No obvious memory leaks
- [ ] **Commit message includes Jira key** (KAN-XX)
- [ ] Jira task status is up to date

---

## 👁️ Code Review Process

### For the Author

**Before requesting review**:
1. Self-review your code
2. Test functionality thoroughly
3. Run norminette
4. Check for memory leaks with valgrind
5. Update documentation if needed
6. Write clear PR description

**PR Description Template**:
```markdown
## Description
Brief description of changes

## Jira Task
- Task: [KAN-XX](https://mohaamedll.atlassian.net/browse/KAN-XX)
- Epic: KAN-X (Epic Name)
- Story Points: X

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation
- [ ] Refactoring

## Implementation Details
- Key files changed
- Algorithm/approach used
- Dependencies on other tasks

## Testing
- [ ] Manual testing completed
- [ ] Norminette passed
- [ ] Valgrind check passed
- [ ] Tested with various inputs
- [ ] Compared with bash behavior
- [ ] All acceptance criteria met

## Checklist
- [ ] Code follows 42 Norm
- [ ] Comments added to complex sections
- [ ] No memory leaks
- [ ] Documentation updated
- [ ] Task moved to "Code Review" in Jira

## Related Tasks
- Depends on: KAN-XX
- Blocks: KAN-XX
- Related: KAN-XX
```

**After PR Approval**:
1. Merge to main
2. Move Jira task to "Testing"
3. Verify task acceptance criteria
4. Move task to "Done"
5. Update sprint progress

### For the Reviewer

**Review Checklist**:

**Functionality**:
- [ ] Code does what it's supposed to do
- [ ] Edge cases are handled
- [ ] Error handling is appropriate
- [ ] No obvious bugs

**Code Quality**:
- [ ] Follows 42 Norm
- [ ] Functions are not too long (25 lines max)
- [ ] Variable names are clear
- [ ] No code duplication
- [ ] Comments are helpful and accurate

**Memory & Resources**:
- [ ] No memory leaks
- [ ] Proper error cleanup
- [ ] File descriptors are closed
- [ ] Memory is freed in correct order

**Testing**:
- [ ] Functionality is testable
- [ ] Author tested adequately
- [ ] No breaking changes to existing code

**Review Comments Guidelines**:
- Be constructive and respectful
- Explain the "why" not just the "what"
- Suggest solutions, not just problems
- Praise good code
- Ask questions if unclear

**Comment Types**:
- 🔴 **MUST FIX**: Critical issue (norm, bug, leak)
- 🟡 **SHOULD FIX**: Important improvement
- 🟢 **CONSIDER**: Suggestion for improvement
- 💡 **TIP**: Educational comment
- 👍 **NICE**: Praise good code

**Example Comments**:
```
🔴 MUST FIX: Memory leak here. Need to free tokens before returning.

🟡 SHOULD FIX: This function is 40 lines. Consider splitting into
helper functions to meet norm requirements.

🟢 CONSIDER: Could use ft_strdup() here instead of manual malloc/strcpy.

💡 TIP: Using ft_split() would simplify this parsing logic.

👍 NICE: Great error handling! Very thorough.
```

### Approval Process

1. **Reviewer tests code locally**
2. **Reviewer comments on issues**
3. **Author addresses comments**
4. **Reviewer approves** (only if all 🔴 and 🟡 resolved)
5. **Merge to main**

---

## 💬 Communication Guidelines

### Daily Standup (15 minutes)

**When**: Start of each work session

**Format**:
1. What did I complete yesterday? (Reference Jira tasks)
2. What will I work on today? (Reference Jira tasks)
3. Any blockers or issues?

**Example**:
```
Member 1:
- Yesterday: ✅ Completed KAN-20 (Environment Getter), tests pass
- Today: 🔄 Working on KAN-45 (Variable Expansion)
- Blockers: None

Member 2:
- Yesterday: ✅ Completed KAN-17 (Makefile), ✅ KAN-18 (Headers)
- Today: 🔄 Starting KAN-38 (Tokenizer implementation)
- Blockers: ⏸️ KAN-41 waiting for KAN-38 completion
```

**Jira Integration**:
- Check board before standup
- Update task statuses
- Add standup notes as comments
- Tag partner on blockers

### Communication Channels

**Synchronous** (real-time):
- In-person meetings
- Video calls
- Screen sharing for debugging

**Asynchronous** (not real-time):
- Pull request comments
- GitHub/GitLab issues
- Commit messages
- Documentation updates

### When to Communicate

**Immediately**:
- You're blocked and can't proceed
- You found a critical bug
- Major design decision needed
- Merge conflicts

**Within 24 hours**:
- Code review requested
- Question about implementation
- Update on progress

**Weekly**:
- Progress summary
- Planning next week's tasks

---

## 📋 Task Assignment Strategy

### Division of Labor

#### Member 1 Primary Areas (Jira Tasks)
**Foundation**:
- KAN-19: Environment List Structure
- KAN-20: Environment Getter
- KAN-21: Environment Cloner
- KAN-22: Environment Setter
- KAN-23: Environment Unset
- KAN-24: Environment Array Conversion

**Lexer**:
- KAN-37: Token Structure
- KAN-38: Tokenization Function
- KAN-39: Quote Handling
- KAN-40: Operator Recognition

**Built-ins**:
- KAN-30: echo command
- KAN-33: export command
- KAN-36: env command

**Expansion**:
- KAN-45: Variable Expansion
- KAN-46: Exit Status Expansion
- KAN-47: Quote Removal

**Redirections**:
- KAN-51: Heredoc Implementation

**Execution**:
- KAN-56: Built-in vs External Routing

**Integration**:
- KAN-57: Main Shell Loop
- KAN-58: Command History
- KAN-59: Prompt Display
- KAN-69: Final Documentation

**Bonus**:
- KAN-66: AND Operator (&&)
- KAN-67: OR Operator (||)

**Total**: ~30 tasks, ~145 story points

#### Member 2 Primary Areas (Jira Tasks)
**Foundation**:
- KAN-16: Project Directory Structure
- KAN-17: Makefile
- KAN-18: Header Files
- KAN-25: Signal Handlers
- KAN-26: String Utilities
- KAN-27: Memory Management
- KAN-28: Input Validation
- KAN-29: Error Handling

**Parser**:
- KAN-41: AST Node Structure
- KAN-42: Parser Main Function
- KAN-43: Syntax Validation
- KAN-44: Redirection Parser

**Built-ins**:
- KAN-31: cd command
- KAN-32: pwd command
- KAN-34: unset command
- KAN-35: exit command

**Redirections**:
- KAN-48: Input Redirection (<)
- KAN-49: Output Redirection (>)
- KAN-50: Append Redirection (>>)
- KAN-52: Pipe Management

**Execution**:
- KAN-53: Process Creation
- KAN-54: Command Execution (execve)
- KAN-55: Process Waiting

**Testing**:
- KAN-60: Built-in Tests
- KAN-61: Pipeline Tests
- KAN-62: Redirection Tests
- KAN-63: Memory Leak Tests
- KAN-64: Norm Compliance
- KAN-65: Test Suite Runner

**Final**:
- KAN-70: Final Testing
- KAN-71: Submission Prep

**Bonus**:
- KAN-68: Logical Operator Tests

**Total**: ~29 tasks, ~140 story points

#### Shared (Pair Programming)
- Complex algorithm design
- Debugging difficult issues
- Architecture decisions
- Integration points between modules
- Performance optimization
- Final testing and validation

### Task Selection Guidelines

**Choose tasks based on**:
1. **Dependencies**: What's blocking other work?
2. **Difficulty**: Balance easy and challenging tasks
3. **Interest**: Work on what excites you
4. **Learning**: Try new areas to grow
5. **Efficiency**: Use your strengths

**Avoid**:
- Both working on dependent tasks simultaneously
- Both working on same file (merge conflicts)
- Leaving critical path tasks until last

### Task Status Updates

**In Jira**:
- Drag tasks between columns on board
- Update status in task view
- Add comments for progress updates
- Log time spent (optional)

**Status Meanings**:
- **To Do**: Ready to start, dependencies met
- **In Progress**: Actively coding
- **Code Review**: PR created, awaiting review
- **Testing**: Merged, being tested
- **Done**: Complete and verified

**Update Frequency**:
- Status: When it changes
- Comments: Daily progress notes
- Blockers: Immediately when encountered
- Completion: When all acceptance criteria met

---

## 🔥 Conflict Resolution

### Code Conflicts

**Prevention**:
1. Work on different modules/files
2. Pull from main frequently
3. Communicate about overlapping work
4. Keep branches short-lived (< 3 days)

**Resolution**:
```bash
# Update your branch with latest main
git checkout feature/my-feature
git fetch origin
git merge origin/main

# Resolve conflicts in editor
# Look for <<<<<<, ======, >>>>>>

# Test after resolving
make && ./minishell

# Commit the merge
git add .
git commit -m "fix: resolve merge conflicts with main"
git push origin feature/my-feature
```

### Design Disagreements

**Process**:
1. **Listen**: Understand other's perspective
2. **Explain**: Share your reasoning
3. **Research**: Check bash behavior, docs
4. **Prototype**: Try both approaches if needed
5. **Decide**: Choose based on:
   - Norm compliance
   - Performance
   - Maintainability
   - Project requirements
6. **Document**: Record decision and reasoning

**Example**:
```
Issue: Should we use array or linked list for environment?

Member 1: Array is faster for lookup
Member 2: List is easier for add/remove

Research: Bash uses hash table
Decision: Start with linked list (simpler), optimize later if needed
Reasoning: Premature optimization. List meets requirements. Can refactor.
```

### Time Management Conflicts

**If falling behind**:
1. Identify bottleneck
2. Reprioritize tasks
3. Ask for help
4. Consider reducing scope (bonus features)
5. Communicate with evaluation partner

---

## ✅ Quality Standards

### Code Quality

**All code must**:
- Follow 42 Norm (check with norminette)
- Have no memory leaks (check with valgrind)
- Compile without warnings (`-Wall -Wextra -Werror`)
- Handle errors gracefully
- Be readable and maintainable

**Function Guidelines**:
- **Max 25 lines** per function
- **Max 4 parameters** per function
- **Single responsibility**: One function, one job
- **Clear naming**: Function name describes what it does
- **Comments**: For complex logic only

**Example of Good Function**:
```c
/*
** Extracts variable name from string starting after '$'
** Returns: Allocated string with variable name, or NULL on error
** Note: Caller must free returned string
*/
char	*extract_var_name(const char *str)
{
	int		i;
	int		len;
	char	*name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	len = i;
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}
```

### Testing Standards

**Before marking task complete**:

1. **Unit Test**: Test function in isolation
2. **Integration Test**: Test with other modules
3. **Edge Cases**: Empty input, NULL, long strings, etc.
4. **Error Cases**: Invalid input, malloc failure, etc.
5. **Bash Comparison**: Compare output with bash

**Test Documentation**:
```bash
# tests/test_env.sh

# Test 1: Basic environment loading
echo "Test 1: Environment initialization"
./minishell <<EOF
env | grep USER
exit
EOF
# Expected: Shows USER variable

# Test 2: Set new variable
echo "Test 2: Set environment variable"
./minishell <<EOF
export TEST=value
echo \$TEST
exit
EOF
# Expected: value

# Test 3: Unset variable
echo "Test 3: Unset variable"
./minishell <<EOF
export TEST=value
unset TEST
echo \$TEST
exit
EOF
# Expected: (empty line)
```

### Documentation Standards

**Code Comments**:
- Explain **why**, not **what**
- Comment complex algorithms
- Document assumptions
- Note edge cases

**Function Documentation**:
```c
/*
** Brief description of function
** Parameters:
**   param1: description
**   param2: description
** Returns: description
** Notes: special behavior, edge cases
*/
```

**Module Documentation**:
- Purpose of module
- Key functions
- Data structures used
- Dependencies
- Usage examples

---

## 📊 Progress Tracking

### Weekly Review (Sprint Review)

**Every Week** (End of Sprint):
1. Review sprint in Jira (burndown chart)
2. Demo completed features
3. Close completed sprint
4. Plan next sprint tasks
5. Update velocity metrics
6. Celebrate wins! 🎉

**Weekly Template**:
```markdown
## Sprint X Review (Week X - Date)

### Sprint Goal 🎯
Complete Foundation & Environment Management (Epic KAN-4)

### Completed ✅ (Story Points)
- KAN-16: Project Structure (2 pts)
- KAN-17: Makefile (3 pts)
- KAN-18: Header Files (3 pts)
- KAN-19: Environment List (5 pts)
- KAN-20: Environment Getter (3 pts)
- KAN-25: Signal Handlers (5 pts)
**Total**: 21/30 story points

### In Progress 🔄
- KAN-38: Tokenization (13 pts, 60% done)
- KAN-41: AST Structure (5 pts, just started)

### Carried Over to Next Sprint ⏭️
- KAN-26: String Utilities (8 pts)
- KAN-37: Token Structure (3 pts)

### Blockers 🚧
- readline installation issue on macOS (resolved)
- Need to finalize header file structure before parser

### Velocity 📊
- Planned: 30 pts
- Completed: 21 pts
- Velocity: 21 pts/week

### Lessons Learned 💡
- Underestimated tokenization complexity
- Pair programming on KAN-25 saved time
- Need to commit more frequently
- Jira time tracking helps with estimation

### Next Sprint Goal 🎯
Complete Lexer (KAN-11) and start Parser (KAN-12)
Target: 25-30 story points

### Team Retrospective
**What went well**: 
- Good communication on dependencies
- Early testing caught issues

**What to improve**:
- Break down large tasks (13+ pts)
- Update Jira more frequently

**Action Items**:
- [ ] Split KAN-38 into subtasks
- [ ] Daily Jira status updates
```

**Use Jira Reports**:
- Sprint Burndown Chart
- Velocity Chart
- Cumulative Flow Diagram

---

## 🎯 Success Criteria

### Code is Ready for Evaluation When:

- [ ] All mandatory features implemented
- [ ] Norminette passes on all files
- [ ] No memory leaks (except readline)
- [ ] No crashes or undefined behavior
- [ ] All test scripts pass
- [ ] Bash compatibility verified
- [ ] Signal handling works correctly
- [ ] All built-ins work as expected
- [ ] Documentation is complete
- [ ] Code is clean (no debug statements)

### Bonus is Ready When:

- [ ] Mandatory is **perfect** (not just "done")
- [ ] All bonus features implemented
- [ ] Bonus features tested thoroughly
- [ ] No regressions in mandatory features

---

## 📚 Resources for Teams

### Time Management
- [Pomodoro Technique](https://en.wikipedia.org/wiki/Pomodoro_Technique)
- [Task Prioritization Matrix](https://www.eisenhower.me/eisenhower-matrix/)

### Git
- [Git Branching Model](https://nvie.com/posts/a-successful-git-branching-model/)
- [Conventional Commits](https://www.conventionalcommits.org/)

### Code Review
- [How to Review Code](https://google.github.io/eng-practices/review/reviewer/)
- [The Art of Code Review](https://www.alexandra-hill.com/2018/06/25/the-art-of-giving-and-receiving-code-reviews/)

### Pair Programming
- [Pair Programming Guide](https://martinfowler.com/articles/on-pair-programming.html)

---

## 💪 Team Principles

1. **Communication is Key**: Over-communicate rather than under-communicate
2. **Ask for Help Early**: Don't struggle alone for hours
3. **Review Each Other's Code**: Two sets of eyes catch more bugs
4. **Respect Each Other's Time**: Be punctual, meet deadlines
5. **Share Knowledge**: Help each other learn and grow
6. **Celebrate Wins**: Acknowledge good work and progress
7. **Learn from Mistakes**: Failures are learning opportunities
8. **Quality Over Speed**: Better to do it right than do it fast
9. **Trust Your Partner**: Both are invested in success
10. **Use Jira Religiously**: Keep board updated for transparency
11. **Have Fun**: This is a learning journey, enjoy it! 🚀

---

## 📱 Quick Reference

### Jira Access
- **URL**: https://mohaamedll.atlassian.net
- **Project Key**: KAN
- **Board**: Minishell Kanban Board

### Key Jira Tasks by Phase

**Week 1 - Foundation**:
- KAN-16 to KAN-29 (14 tasks)

**Week 2 - Lexer**:
- KAN-37 to KAN-40 (4 tasks)

**Week 3 - Parser & Built-ins**:
- KAN-30 to KAN-36 (Built-ins: 7 tasks)
- KAN-41 to KAN-44 (Parser: 4 tasks)

**Week 4 - Expansion & Redirections**:
- KAN-45 to KAN-47 (Expansion: 3 tasks)
- KAN-48 to KAN-51 (Redirections: 4 tasks)

**Week 5 - Execution & Pipes**:
- KAN-52 (Pipes: 1 task)
- KAN-53 to KAN-56 (Execution: 4 tasks)

**Week 6 - Integration**:
- KAN-57 to KAN-59 (Integration: 3 tasks)
- KAN-60 to KAN-62 (Tests: 3 tasks)

**Week 7 - Testing & Docs**:
- KAN-63 to KAN-65 (Testing: 3 tasks)
- KAN-69 to KAN-71 (Final: 3 tasks)

**Week 8 - Bonus (Optional)**:
- KAN-66 to KAN-68 (3 tasks)

### Task Dependencies Map

```
Foundation (Week 1)
├── KAN-18 (Headers) → All modules depend on this
├── KAN-19 (Env List) → KAN-20, KAN-21, KAN-22, KAN-23, KAN-24
├── KAN-26 (String Utils) → Used everywhere
└── KAN-25 (Signals) → KAN-57 (Main Loop)

Lexer (Week 2)
└── KAN-37 (Tokens) → KAN-38, KAN-39, KAN-40 → KAN-42 (Parser)

Parser (Week 3)
└── KAN-41 (AST) → KAN-42, KAN-43, KAN-44 → Execution

Built-ins (Week 3)
└── KAN-30 to KAN-36 → KAN-56 (Router)

Expansion (Week 4)
└── KAN-45 (Var Expand) → KAN-46, KAN-47

Redirections (Week 4-5)
└── KAN-48, KAN-49, KAN-50 → Independent
└── KAN-51 (Heredoc) → Complex, depends on KAN-45

Execution (Week 5)
└── KAN-53 (Fork) → KAN-54 (Exec) → KAN-55 (Wait) → KAN-56 (Router)

Integration (Week 6)
└── All previous tasks → KAN-57 (Main Loop)

Testing (Week 6-7)
└── All features → KAN-60 to KAN-65
```

### Emergency Contacts

**Blockers**: Tag partner in Jira immediately  
**Git Issues**: Schedule pair programming session  
**Technical Questions**: Add comment in relevant Jira task  
**Sprint Planning**: Weekly meeting (end of sprint)

---

## 🔧 Troubleshooting Jira-Git Integration

### Development Info Not Showing in Jira?

**Check these common issues**:

1. **Wrong Key Format**
   - ❌ Wrong: `kan-23`, `Kan-23`, `kan23`
   - ✅ Correct: `KAN-23`

2. **Key Position in Commit**
   - ❌ Wrong: `implement function KAN-23`
   - ✅ Correct: `KAN-23: implement function`
   - ✅ Also works: `KAN-23 implement function`

3. **Not Pushed Yet**
   - Git info only syncs after `git push`
   - Wait 1-2 minutes after pushing

4. **GitHub/Git Integration Not Set Up**
   - Check with Jira admin
   - Verify GitHub app is installed
   - Confirm repository is connected

5. **Branch Not Synced**
   - Ensure branch is pushed: `git push origin branch-name`
   - Check branch name includes key: `KAN-23-description`

**Quick Test**:
```bash
# Create test commit
git commit -m "KAN-16: test jira integration" --allow-empty
git push

# Wait 1-2 minutes, then check KAN-16 in Jira
# Should see commit in Development section
```

### PR Not Linking to Jira?

**Common fixes**:
1. **Add key to PR title**: `KAN-23: Description`
2. **Ensure branch name has key**: `KAN-23-branch-name`
3. **Wait for sync**: Can take 2-3 minutes
4. **Check GitHub/Jira connection**: Verify in Jira settings

### Can't Find Task Key?

**Where to find Jira keys**:
1. **On Board**: Bottom of each card (e.g., `KAN-23`)
2. **In Task**: Breadcrumb at top (e.g., `Projects / KAN / KAN-23`)
3. **URL**: `https://mohaamedll.atlassian.net/browse/KAN-23`
4. **Search**: Use Jira search bar

### Git Commands Reference

**Create branch with Jira key**:
```bash
git checkout -b KAN-23-short-description
```

**Commit with Jira key**:
```bash
git commit -m "KAN-23: commit message"
```

**Push and link**:
```bash
git push origin KAN-23-short-description
```

**Check remote branch**:
```bash
git branch -r | grep KAN-23
```

---

## 📚 Additional Resources

### Jira Documentation
- [Atlassian Jira Guide](https://support.atlassian.com/jira-software-cloud/)
- [Integrating Development Tools](https://support.atlassian.com/jira-cloud-administration/docs/integrate-with-development-tools/)
- [Smart Commits](https://support.atlassian.com/jira-software-cloud/docs/process-issues-with-smart-commits/)

### Git Best Practices
- [Conventional Commits](https://www.conventionalcommits.org/)
- [Git Branch Naming](https://dev.to/varbsan/a-simplified-convention-for-naming-branches-and-commits-in-git-il4)

### Team Collaboration
- [Agile Sprint Planning](https://www.atlassian.com/agile/scrum/sprint-planning)
- [Story Point Estimation](https://www.atlassian.com/agile/project-management/estimation)

---

**Document Version**: 2.1  
**Last Updated**: November 3, 2025  
**For**: Minishell Team  
**Jira Project**: KAN (mohaamedll.atlassian.net)  
**GitHub Integration**: ✅ Enabled
