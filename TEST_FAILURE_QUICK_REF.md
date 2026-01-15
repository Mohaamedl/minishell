# Quick Reference - PR #52 Test Failures

## TL;DR

**Problem:** Tests failing due to extra trailing spaces in output  
**Status:** 3 jobs failed, 9 tests total  
**Severity:** Medium (output formatting issue)  
**Success Rate:** 87% of tests still pass

## Failure Pattern

```
Input:  echo "*.txt"
Expect: *.txt
Got:    *.txt ←─── EXTRA SPACE
             ^
```

## Failed Tests

1. **Wildcards** (3 tests) - Quoted patterns have trailing space
2. **Variable Expansion** (3 tests) - Quoted variables have trailing space  
3. **Echo Builtin** (3 tests) - Empty/special strings have extra space

## Root Cause

❌ **NOT** in echo builtin (code is correct)  
✅ **LIKELY** in quote handling or tokenization

## What to Check

```
src/tokenizer/create_token.c         ← Token value creation
src/tokenizer/handle_word_token.c    ← Word processing
src/expander/var_expand.c            ← Quote removal
src/executer/execute_cmd.c           ← Argument array building
```

## Quick Fix Checklist

- [ ] Add debug output to `builtin_echo()` to see args
- [ ] Check token values during tokenization
- [ ] Verify quote removal doesn't add spaces
- [ ] Check argument array construction
- [ ] Test locally: `echo "*.txt"` should output `*.txt` not `*.txt `
- [ ] Run affected test suites
- [ ] Push fix and verify CI passes

## Test Commands

```bash
# Failing tests
./minishell -c 'echo "*.txt"'      # Should: *.txt
./minishell -c 'echo "$VAR"'       # Should: value
./minishell -c 'echo ""'           # Should: empty

# Test suites to run after fix
bash tests/builtins/test_echo.sh
bash tests/expander/test_expansion.sh
bash tests/expander/test_wildcard.sh
```

## CI Job Results

| Category | Status | Pass Rate |
|----------|--------|-----------|
| Build & Style | ✅ | 100% |
| Memory Leaks | ✅ | 100% |
| Most Builtins | ✅ | 100% |
| Echo Tests | ❌ | 87% |
| Expander Tests | ❌ | 87% |
| Wildcard Tests | ❌ | 85% |

## Documents

- **Full Analysis:** `TEST_FAILURE_ANALYSIS_PR52.md`
- **Technical Guide:** `TEST_FAILURE_TECHNICAL_NOTES.md`
- **This Summary:** `TEST_FAILURE_QUICK_REF.md`

## Next Actions

1. Review full analysis report
2. Follow debugging strategy in technical notes
3. Add debug output to identify exact location
4. Apply fix (likely 1-2 line change)
5. Verify with test suites
6. Re-run CI pipeline

---

**PR:** #52 "Kan 81 norminette tokenizer"  
**Workflow:** 20817062387  
**Date:** 2026-01-08  
**Analysis Date:** 2026-01-15
