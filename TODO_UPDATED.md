# 🚀 Minishell - Updated TODO

**Last Updated**: November 19, 2025 20:35  
**Status**: 🔴 Critical blocker discovered - Execution engine missing

---

## 🚨 CRITICAL - Do IMMEDIATELY

### ⚠️ Phase 4: Execution Engine (BLOCKING EVERYTHING)

**Discovery**: After running comprehensive tests (173+ test cases), we found that the parser builds the AST correctly but **never executes it**. All tests fail because commands are parsed but not run.

**Evidence**: 
```bash
$ echo "echo hello" | ./minishell
echo    # ← Only prints command name, doesn't execute
```

**Impact**: 0% of functionality tests pass (0/173 tests)

---

### Priority 1: Basic Command Execution (2-3 days)
**Estimated Time**: 8-12 hours  
**Assignee**: BOTH (pair programming recommended)

- [ ] **Create executor module**
  - [ ] Create `src/executor/` directory
  - [ ] Create `executor.c` and `executor.h`
  - [ ] Add to Makefile

- [ ] **Implement `execute_ast()` function**
  ```c
  int execute_ast(t_ast *node, t_shell *shell)
  {
      if (node->type == CMD_NODE)
          return execute_command(node, shell);
      if (node->type == PIPE_NODE)
          return execute_pipe(node, shell);
      if (node->type == REDIRECT_NODE)
          return execute_redirect(node, shell);
      return 0;
  }
  ```

- [ ] **Implement `execute_command()`**
  ```c
  int execute_command(t_ast *node, t_shell *shell)
  {
      char **args = node->args;  // Get args from AST
      
      if (is_builtin(args[0]))
          return execute_builtin(args, shell);  // Already exists!
      else
          return execute_external(args, shell); // TODO later
  }
  ```

- [ ] **Update main.c**
  ```c
  // Current (WRONG):
  print_tree(root_node);
  
  // New (CORRECT):
  shell.last_exit_status = execute_ast(root_node, &shell);
  ```

- [ ] **Test basic execution**
  - [ ] Test: `echo hello` should print "hello"
  - [ ] Test: `pwd` should print current directory
  - [ ] Test: `env` should print environment
  - [ ] Run full test suite - expect ~80% pass rate

**Acceptance Criteria**:
- ✅ Basic builtins work from command line
- ✅ At least 100/173 tests pass
- ✅ Can chain commands with `;` (if AST supports it)

---

### Priority 2: Pipeline Execution (3-4 days)
**Estimated Time**: 12-15 hours  
**Depends on**: Priority 1 complete

- [ ] **Implement `execute_pipe()`**
  - [ ] Create pipe with `pipe()`
  - [ ] Fork for left command
  - [ ] Fork for right command  
  - [ ] Connect stdout → stdin via pipe
  - [ ] Wait for both processes

- [ ] **Test pipes**
  - [ ] Test: `echo hello | cat` 
  - [ ] Test: `ls | grep test`
  - [ ] Test: `cat file | wc -l | cat`

**Acceptance Criteria**:
- ✅ Simple pipes work (`cmd1 | cmd2`)
- ✅ Multiple pipes work (`cmd1 | cmd2 | cmd3`)
- ✅ Pipe tests pass

---

### Priority 3: Redirections (2-3 days)
**Estimated Time**: 8-10 hours  
**Depends on**: Priority 1 complete

- [ ] **Implement `execute_redirect()`**
  - [ ] Handle `<` (input from file)
  - [ ] Handle `>` (output to file, truncate)
  - [ ] Handle `>>` (output to file, append)
  - [ ] Open files with proper permissions
  - [ ] Redirect file descriptors
  - [ ] Close files after use

- [ ] **Test redirections**
  - [ ] Test: `echo hello > file`
  - [ ] Test: `cat < file`
  - [ ] Test: `ls >> file`

**Acceptance Criteria**:
- ✅ All redirection operators work
- ✅ File permissions correct (644 for output)
- ✅ Error handling for file operations

---

### Priority 4: Here-doc (1-2 days)
**Estimated Time**: 4-6 hours  
**Depends on**: Priority 3 complete

- [ ] **Implement `<<` operator**
  - [ ] Detect here-doc delimiter
  - [ ] Read input until delimiter
  - [ ] Create temporary file
  - [ ] Use as input redirection

- [ ] **Test here-doc**
  - [ ] Test: `cat << EOF`
  - [ ] Test with variable expansion

---

### Priority 5: External Commands (1-2 days)
**Estimated Time**: 6-8 hours  
**Depends on**: Priority 1 complete

- [ ] **Implement `execute_external()`**
  - [ ] Search PATH for executable
  - [ ] Fork new process
  - [ ] Call `execve()`
  - [ ] Handle errors (command not found)

- [ ] **Test external commands**
  - [ ] Test: `/bin/ls`
  - [ ] Test: `ls` (PATH resolution)
  - [ ] Test: `cat`, `grep`, `wc`

---

## ✅ COMPLETED

### Phase 1: Foundation (100%) - Merged
- ✅ Environment management (4 files)
- ✅ Signal handling
- ✅ Project structure
- ✅ Makefile
- ✅ Headers

**Tests**: 9/9 passing ✅

### Phase 2: Parser (100%) - Merged
- ✅ Tokenizer (5 files)
- ✅ AST builder (4 files)  
- ✅ Command/operator lists (5 files)
- ✅ Tree printing for debug

**Status**: Working perfectly, confirmed by tests

### Phase 3: Built-ins (100%) - Merged
- ✅ echo, cd, pwd, export, unset, env, exit (8 files)
- ✅ Comprehensive implementations
- ✅ Error handling
- ✅ Edge cases covered

**Status**: Implemented but untestable without executor

### Testing Infrastructure (100%)
- ✅ Master test runner (`run_all_tests.sh`)
- ✅ Individual test files (7 files)
- ✅ 173+ test cases
- ✅ Colored output, statistics

**Status**: Tests ready, waiting for execution engine

---

## 📊 Progress Tracking

| Phase | Code | Tests | Status |
|-------|------|-------|--------|
| Phase 1: Foundation | 100% | 100% | ✅ Done |
| Phase 2: Parser | 100% | N/A | ✅ Done |
| Phase 3: Builtins | 100% | 0% | 🔴 Blocked |
| **Phase 4: Executor** | **0%** | **0%** | 🔴 **BLOCKER** |
| Phase 5: Integration | 0% | 0% | ⏳ Waiting |
| **OVERALL** | **60%** | **11%** | 🟡 **In Progress** |

---

## 🎯 This Week's Goals

### Monday-Tuesday
- [ ] Implement basic `execute_ast()`
- [ ] Connect to main loop
- [ ] Test with simple commands

### Wednesday-Thursday
- [ ] Refine command execution
- [ ] Add error handling
- [ ] Run full test suite
- [ ] Debug failing tests

### Friday
- [ ] Start pipeline implementation
- [ ] Update documentation
- [ ] Commit and push executor

**Target**: 100+ tests passing by end of week

---

## 🎓 Lessons Learned

1. **Integration Testing**: Should have tested end-to-end after parser merge
2. **Branch Coordination**: Parser and builtins weren't connected
3. **Code Review**: AST printing hid the missing executor
4. **Test Early**: Waiting to test found issue late

---

## 📝 Notes

### Why Everything is "Done" but Tests Fail
- Parser: ✅ Works perfectly (tokenizes and builds AST)
- Builtins: ✅ Implemented correctly
- **Missing Link**: No code to traverse AST and call builtins
- Main loop: Builds tree → **prints it** → frees it (no execution!)

### The Simple Fix
Replace ONE line in `main.c`:
```c
- print_tree(root_node);           // Debug only
+ execute_ast(root_node, &shell);  // Actually run commands
```

Then implement `execute_ast()` to traverse tree and call functions.

### Estimated Time to Working Shell
- Basic executor: 2-3 days (builtins work)
- + Pipes: 3-4 days  
- + Redirections: 2-3 days
- + External commands: 1-2 days
- **Total**: 8-12 days to full functionality

---

**Next Session**: Start implementing `execute_ast()` - pair programming recommended

*Updated: Nov 19, 2025 20:35 | Critical blocker identified and prioritized*
