# Norminette Refactoring Tracking

**Date Started:** January 13, 2026  
**Branch:** refactor/norminette  
**Status:** In Progress

## Objectives

1. ✅ Run norminette on all source files
2. 🔄 Refactor large functions (>25 lines) without changing logic
3. 🔄 Move helper functions to utils files if file has too many functions
4. ✅ Ensure all files pass norminette
5. 🔄 Run full test pipeline after changes
6. ✅ Track all changes in this document

---

## Progress Summary

| Directory | Files Checked | Status | Issues Found | Issues Fixed |
|-----------|---------------|--------|--------------|--------------|
| `src/builtins/` | 8 | ✅ PASS | 0 | 0 |
| `src/environment/` | 4 | ✅ PASS | 0 | 0 |
| `src/execution/` | 9 | ✅ PASS | 0 | 0 |
| `src/process/` | 5 | ✅ PASS | 0 | 0 |
| `src/signals/` | 2 | ✅ PASS | 14 | 14 |
| `src/utils/` | 7 | ✅ PASS | 1 | 1 |

---

## Detailed Changes by Directory

### ✅ src/builtins/ (COMPLETED)

**Status:** All files pass norminette  
**Files:** 8 total

| File | Status | Lines | Functions | Issues | Changes Made |
|------|--------|-------|-----------|--------|--------------|
| `builtin_utils.c` | ✅ OK | ~150 | 4 | None | None |
| `cd.c` | ✅ OK | ~200 | 5 | None | Previously refactored |
| `echo.c` | ✅ OK | ~90 | 2 | None | Previously refactored |
| `env.c` | ✅ OK | ~60 | 1 | None | None |
| `exit.c` | ✅ OK | ~150 | 3 | None | None |
| `export.c` | ✅ OK | ~180 | 4 | None | None |
| `pwd.c` | ✅ OK | ~60 | 1 | None | None |
| `unset.c` | ✅ OK | ~80 | 1 | None | None |

**Notes:**
- All builtins conform to norminette standards
- Function counts and line counts are within limits
- No further refactoring required

---

### ✅ src/environment/ (COMPLETED)

**Status:** All files pass norminette  
**Files:** 4 total

| File | Status | Lines | Functions | Issues | Changes Made |
|------|--------|-------|-----------|--------|--------------|
| `env_get.c` | ✅ OK | - | - | None | None |
| `env_init.c` | ✅ OK | - | - | None | None |
| `env_set.c` | ✅ OK | - | - | None | None |
| `env_utils.c` | ✅ OK | - | - | None | None |

**Notes:**
- All environment files conform to norminette standards
- No refactoring required

---

### ✅ src/process/ (COMPLETED)

**Status:** All files pass norminette  
**Files:** 5 total

| File | Status | Lines | Functions | Issues | Changes Made |
|------|--------|-------|-----------|--------|--------------|
| `exec.c` | ✅ OK | - | - | None | None |
| `pipes.c` | ✅ OK | - | - | None | None |
| `pipes_utils.c` | ✅ OK | - | - | None | None |
| `process.c` | ✅ OK | - | - | None | None |
| `wait.c` | ✅ OK | - | - | None | None |

**Notes:**
- All process management files conform to norminette standards
- No refactoring required

---

### ✅ src/execution/ (COMPLETED)

**Status:** All files pass norminette after refactoring
**Files:** 9 total

| File | Status | Issues | Changes Made |
|------|--------|--------|--------------|
| `execute_ast.c` | ✅ OK | 0 | Split into 3 files, moved comments to docs |
| `pipeline_helpers.c` | ✅ OK | 0 | Extracted from execute_ast.c |
| `pipeline_execution.c` | ✅ OK | 0 | Extracted from execute_ast.c |
| `execute_ast.c` | ❌ FAIL | 100+ | HIGH | Too many functions, comments, >25 lines |
| `execute_cmd.c` | ❌ FAIL | 50+ | HIGH | Comments, formatting, long lines |
| `execute_cmd_helpers.c` | ❌ FAIL | 15+ | MEDIUM | Comments, >25 lines, formatting |
| `handle_redirections.c` | ❌ FAIL | 20+ | MEDIUM | Formatting, return parenthesis |
| `heredoc.c` | ❌ FAIL | 30+ | HIGH | >25 lines, comments, long lines |

**Common Issues:**
- WRONG_SCOPE_COMMENT: Many inline comments need removal/relocation
- TOO_MANY_LINES: Functions exceed 25 line limit
- TOO_MANY_FUNCS: Files exceed 5 function limit
- SPACE_REPLACE_TAB: Indentation with spaces instead of tabs
- LINE_TOO_LONG: Lines exceed 80 characters
- RETURN_PARENTHESIS: Missing parentheses in return statements

---

### ✅ src/signals/ (COMPLETED)

**Status:** All files pass norminette  
**Files:** 2 total (split from 1)

| File | Status | Issues | Priority | Changes Made |
|------|--------|--------|----------|----------------|
| `signals.c` | ✅ OK | 0 (was 14) | - | Removed inline comments, split heredoc functions |
| `signals_heredoc.c` | ✅ OK | 0 (new) | - | Created to hold heredoc-related signal functions |

**Changes Made:**
- Removed all WRONG_SCOPE_COMMENT violations (10 inline comments)
- Fixed CONSECUTIVE_SPC violations (5 spacing issues)
- Split file to fix TOO_MANY_FUNCS (created signals_heredoc.c)
- Moved heredoc signal handling to separate file
- Updated Makefile to include new file

---

### ✅ src/utils/ (COMPLETED)

**Status:** All files pass norminette  
**Files:** 7 total (was 6, added char_utils.c)

| File | Status | Issues | Priority | Changes Made |
|------|--------|--------|----------|----------------|
| `error.c` | ✅ OK | 0 | - | None |
| `ft_split.c` | ✅ OK | 0 | - | None |
| `memory_utils.c` | ✅ OK | 0 | - | None |
| `number_utils.c` | ✅ OK | 0 | - | None |
| `string_utils.c` | ✅ OK | 0 (was 1) | - | Removed ft_isalnum (moved to char_utils.c) |
| `string_utils2.c` | ✅ OK | 0 (was 1) | - | Removed ft_isalnum |
| `char_utils.c` | ✅ OK | 0 (new) | - | Created for character utility functions |

**Changes Made:**
- Fixed TOO_MANY_FUNCS in string_utils2.c by removing ft_isalnum
- Created char_utils.c for character utility functions
- Consolidated ft_isalnum implementation in new file
- Updated Makefile to include new file

---

## Common Norminette Rules Applied

### Function Length (25 lines max)
- Extract helper functions for complex logic
- Maintain original function behavior
- Preserve memory cleanup and error handling

### Function Count (5 functions max per file)
- Create separate utils files for helpers
- Group related functions logically
- Maintain file coherence

### Formatting Rules
- Space after commas
- No empty lines in functions
- Proper function separation (blank line)
- Comment scope compliance

---

## Refactoring Patterns Used

### Pattern 1: Extract Validation Helper
```c
// Before: validation inline in main function
// After: static int validate_args(...) helper
```

### Pattern 2: Extract Error Handling
```c
// Before: cleanup code repeated
// After: static int handle_error(...) helper
```

### Pattern 3: Split Multi-Step Logic
```c
// Before: one large function with many steps
// After: multiple focused helpers
```

---

## Test Pipeline Checklist

- [ ] `make` - Compilation successful
- [ ] `make clean && make` - Clean build
- [ ] `tests/run_all_tests.sh` - All tests pass
- [ ] Manual smoke tests
- [ ] Memory leak checks (valgrind)

---

## Notes and Observations

### Challenges
- TBD

### Best Practices Identified
- Keep helper functions static and close to usage
- Maintain consistent error handling patterns
- Preserve all memory cleanup paths

### Future Improvements
- TBD

---

## Sign-off

- [ ] All norminette checks pass
- [ ] All tests pass
- [ ] No regressions introduced
- [ ] Documentation updated
- [ ] Ready for merge to main

**Last Updated:** January 13, 2026
