# Test Failure Analysis Report - PR #52 (KAN-81 Norminette Tokenizer)

**Pull Request:** #52 - "Kan 81 norminette tokenizer"  
**Branch:** `KAN-81-norminette-tokenizer`  
**Commit SHA:** `0d24333fd50b8c594639a623750c11f71478a363`  
**Workflow Run ID:** 20817062387  
**Date:** January 8, 2026  
**Status:** ❌ FAILED

---

## Executive Summary

The CI/CD pipeline for PR #52 failed with **3 test jobs failing** out of 18 total jobs. The primary issue appears to be related to **extra trailing spaces in command output**, affecting multiple test categories:

- ✅ **Build**: Successful
- ✅ **Norminette**: Successful (code style checks passed)
- ✅ **Memory Leak Check (Valgrind)**: Successful
- ❌ **Bonus Feature Tests (Wildcards)**: 3 tests failed
- ❌ **Unit Tests (Expander)**: 3 tests failed
- ❌ **Builtin Command Tests (Echo)**: 3 tests failed
- ⚠️ **Unit Tests (AST, Parser, Tokenizer)**: Cancelled (due to earlier failures)

---

## Detailed Failure Analysis

### 1. Bonus Feature Tests (Wildcard Expansion) - Job ID: 59795347946

**Status:** ❌ FAILED  
**Tests Passed:** 17/20 (85%)  
**Tests Failed:** 3

#### Failed Tests:

##### Test 11: Double quotes prevent expansion
- **Input:** `echo "*.txt"`
- **Expected:** `*.txt`
- **Got:** `*.txt ` ← **Extra trailing space**
- **Issue:** Trailing space added to quoted output

##### Test 12: Single quotes prevent expansion
- **Input:** `echo '*.txt'`
- **Expected:** `*.txt`
- **Got:** `*.txt ` ← **Extra trailing space**
- **Issue:** Trailing space added to quoted output

##### Test 13: Quoted asterisk literal
- **Input:** `echo "*"`
- **Expected:** `*`
- **Got:** `* ` ← **Extra trailing space**
- **Issue:** Trailing space added to quoted output

#### Root Cause:
The echo command is adding an extra space after quoted arguments. This suggests an issue with how the echo builtin or the argument processing handles quoted strings.

---

### 2. Unit Tests (Expander - Variable Expansion) - Job ID: 59795347967

**Status:** ❌ FAILED  
**Tests Passed:** 21/24 (87.5%)  
**Tests Failed:** 3

#### Failed Tests:

##### Test 12: Variable in double quotes
- **Input:** `echo "$TEST_VAR"`
- **Expected:** `hello`
- **Got:** `hello ` ← **Extra trailing space**
- **Issue:** Trailing space added after variable expansion in quotes

##### Test 13: Variable in single quotes (no expansion)
- **Input:** `echo '$TEST_VAR'`
- **Expected:** `$TEST_VAR`
- **Got:** `$TEST_VAR ` ← **Extra trailing space**
- **Issue:** Trailing space added to literal string in single quotes

##### Test 14: Mixed quotes
- **Input:** `echo "$TEST_VAR" '$TEST_VAR'`
- **Expected:** `hello $TEST_VAR`
- **Got:** `hello  $TEST_VAR ` ← **Extra space between args AND trailing space**
- **Issue:** Double space between arguments and trailing space at end

#### Root Cause:
Similar to wildcard tests, this is related to how quoted arguments are being processed. The issue manifests when:
1. Variables are expanded within double quotes
2. Literal strings are in single quotes
3. Multiple quoted arguments are used together

---

### 3. Builtin Command Tests (Echo) - Job ID: 59795347972

**Status:** ❌ FAILED  
**Tests Passed:** 21/24 (87.5%)  
**Tests Failed:** 3

#### Failed Tests:

##### Test: Echo empty string
- **Input:** `echo ""`
- **Expected:** `` (empty) or empty string
- **Got:** ` ` ← **Single space instead of empty**
- **Issue:** Empty string produces a space

##### Test: Echo just spaces
- **Input:** `echo "   "`
- **Expected:** `   ` (3 spaces) or ` ` (1 space)
- **Got:** `    ` ← **4 spaces instead of 3**
- **Issue:** Extra space added

##### Test: Echo with dollar (literal)
- **Input:** `echo "hello$"`
- **Expected:** `hello$`
- **Got:** `hello$ ` ← **Extra trailing space**
- **Issue:** Trailing space added to quoted output

#### Root Cause:
The echo builtin is consistently adding an extra space to its output, particularly when:
1. Empty strings are provided
2. Strings containing only spaces are provided
3. Quoted strings with special characters are provided

---

## Common Pattern Identified

### The Trailing Space Problem

All three failing test jobs share a common issue: **extra trailing spaces in output**.

**Pattern observed:**
```
Expected: 'text'
Got:      'text '  ← Extra space
          ^^^^^^
```

This pattern appears in:
- ✓ Quoted wildcard patterns: `"*.txt"` → `*.txt `
- ✓ Quoted variables: `"$VAR"` → `value `
- ✓ Empty quoted strings: `""` → ` `
- ✓ Mixed quote scenarios: Multiple extra spaces

### Likely Root Causes:

1. **Echo Builtin Implementation** (`src/builtins/echo.c`):
   - May be adding an extra space after each argument
   - May not be properly handling the last argument
   - Could be printing a space before the newline

2. **Argument Processing/Expansion**:
   - Quote removal may be leaving extra spaces
   - Argument array building may be adding trailing spaces
   - Word splitting after expansion may introduce spaces

3. **Token Processing**:
   - Tokenizer may be creating tokens with trailing spaces
   - Quote handling during tokenization might preserve extra spaces

---

## Impact Assessment

### Severity: **MEDIUM**

While the failures are consistent and affect multiple test categories, they represent a relatively small percentage of total tests:

- Overall success rate: **~87%** across failing jobs
- Core functionality (build, memory, norminette): ✅ All passing
- Issue is limited to output formatting, not functionality

### Affected Areas:

1. **Echo builtin** - Output formatting
2. **Variable expansion** - Quote handling
3. **Wildcard expansion** - Quote protection
4. **Argument processing** - Space management

---

## Recommended Actions

### Priority 1: Fix Echo Builtin

**Location:** `src/builtins/echo.c`

**Actions:**
1. Review the argument printing loop
2. Ensure spaces are only added between arguments, not after the last one
3. Verify handling of empty strings and quoted strings
4. Check the newline logic (should not have space before `\n`)

**Example fix areas:**
```c
// Check if adding space after every argument including the last
// Should be: space BETWEEN args, not AFTER each arg
```

### Priority 2: Review Argument Processing

**Location:** Likely in expander or tokenizer

**Actions:**
1. Check quote removal logic
2. Verify argument array construction
3. Ensure no trailing spaces are added during expansion
4. Review word splitting after variable/wildcard expansion

### Priority 3: Run Targeted Tests

After fixes, run these specific test suites:
```bash
# Test echo builtin
bash tests/builtins/test_echo.sh

# Test variable expansion
bash tests/expander/test_expansion.sh

# Test wildcard expansion
bash tests/expander/test_wildcard.sh
```

---

## Technical Details

### Workflow Information

- **Workflow:** Minishell CI/CD Pipeline
- **File:** `.github/workflows/ci.yml`
- **Trigger:** Pull request to main branch
- **Runner:** ubuntu-latest (GitHub Actions)

### Job Matrix

| Job Name | Status | Duration | Tests Passed | Tests Failed |
|----------|--------|----------|--------------|--------------|
| Build Minishell | ✅ Success | 24s | - | - |
| Norminette Check | ✅ Success | 9s | - | - |
| Memory Leak Check | ✅ Success | 18s | - | - |
| Unit Tests (ast) | ⚠️ Cancelled | - | - | - |
| Unit Tests (parser) | ⚠️ Cancelled | - | - | - |
| Unit Tests (tokenizer) | ⚠️ Cancelled | - | - | - |
| Bonus Feature Tests | ❌ Failure | 18s | 17 | 3 |
| Builtin Tests (cd) | ✅ Success | 17s | All | 0 |
| Builtin Tests (exit) | ✅ Success | 19s | All | 0 |
| Builtin Tests (env) | ✅ Success | 17s | All | 0 |
| Unit Tests (expander) | ❌ Failure | 19s | 21 | 3 |
| Builtin Tests (echo) | ❌ Failure | 29s | 21 | 3 |
| Builtin Tests (export) | ✅ Success | 24s | All | 0 |
| Builtin Tests (unset) | ✅ Success | 19s | All | 0 |
| Builtin Tests (pwd) | ✅ Success | 18s | All | 0 |
| Final Status Check | ❌ Failure | 3s | - | - |
| Generate Coverage | ✅ Success | 5s | - | - |
| Integration Tests | ⏭️ Skipped | - | - | - |

### Environment

- OS: Ubuntu 24.04 (noble)
- Compiler: gcc (default)
- Readline: libreadline-dev 8.2-4build1
- Valgrind: Installed and configured

---

## Code Areas to Investigate

### 1. Echo Builtin (`src/builtins/echo.c`)

```bash
# Lines to check:
# - Argument iteration loop
# - Space insertion logic
# - Newline handling
# - Quote handling (if done in echo itself)
```

### 2. Variable Expander (`src/expander/var_expand.c`)

```bash
# Lines to check:
# - Quote removal after expansion
# - Argument array building
# - Space handling in expansions
```

### 3. Wildcard Expander (`src/expander/wildcard*.c`)

```bash
# Files: wildcard.c, wildcard_expand.c, wildcard_args.c
# - Quote detection for wildcard prevention
# - Argument construction from matches
# - Space handling in results
```

### 4. Tokenizer (`src/tokenizer/tokenizer.c` and helpers)

```bash
# - Quote handling during tokenization
# - Token value construction
# - Space preservation in quoted tokens
```

---

## Conclusion

The PR #52 pipeline failures are caused by a consistent issue with **trailing spaces in command output**. This affects:

- 3 out of 18 jobs (16.7% failure rate)
- 9 out of 68 tests in failing jobs (13.2% of tests in those jobs)
- All failures follow the same pattern: extra space at end of output

**Positive aspects:**
- ✅ Build is successful
- ✅ Code passes norminette style checks
- ✅ No memory leaks detected
- ✅ Most builtins work correctly (cd, env, exit, export, unset, pwd)
- ✅ 87% of tests in failing jobs still pass

**Resolution strategy:**
1. Fix the echo builtin's space handling
2. Review quote processing in expansion
3. Verify argument array construction
4. Re-run targeted tests
5. Re-run full CI pipeline

**Estimated effort:** Small to medium (likely a few line changes in echo.c)

**Risk level:** Low (isolated to output formatting, not core functionality)

---

## Appendix: Sample Test Outputs

### Wildcard Test (Test 11)
```bash
Test 11: Double quotes prevent expansion
  Input: echo "*.txt"
  Expected: '*.txt'
  Got: '*.txt '
```

### Variable Expansion Test (Test 12)
```bash
Test 12: Variable in double quotes
  Input: echo "$TEST_VAR"
  Expected: 'hello'
  Got: 'hello '
```

### Echo Builtin Test (Empty String)
```bash
Test: Echo empty string
  Input: echo ""
  Expected: '' or empty
  Got: ' '
```

---

**Report Generated:** 2026-01-15  
**Author:** Automated CI/CD Analysis  
**Next Review:** After fixes are applied
