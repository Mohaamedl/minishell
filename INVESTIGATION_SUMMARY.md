# Pipeline Test Failures Investigation Summary

## Overview
This investigation analyzed why pipeline tests were failing on certain norminette branches and identified the root causes.

## Branches Analyzed

### 1. KAN-84-norminette-expander (PR #56) ❌ → ✅ FIXED
**Status**: Was failing, now fixed
**Test Results**: 
- Before: 22/25 export tests passing (88% success rate)
- After: 25/25 export tests passing (100% success rate)

**Issue**: Quote removal logic bug in `src/expander/get_var_value.c`
**Failing Tests**:
- Test 19: Triple double quotes - Expected: `ddsd`, Got: `"ddsd"`
- Test 20: Double empty quotes with unquoted middle - Expected: `ddsd`, Got: `"ddsd"`
- Test 21: Mixed double and single quotes - Expected: `ddsdd`, Got: `"'ddsdd'"`

**Root Cause**: The `remove_quotes()` function used a helper `handle_quotes()` that made the logic overly complex. When encountering complex quote patterns like `""ddsd""` (adjacent empty quotes), the function would incorrectly leave quote characters in the output.

**Fix Applied**: Replaced buggy implementation with cleaner version from KAN-85 branch that:
- Directly handles quote state transitions inline (no helper function)
- Properly tracks quote state with a simple char variable
- Skips quote characters themselves while copying content between them

**Code Changes**:
```c
// Before (buggy):
while (str[i])
{
    handle_quotes(str, &i, &in_quote);
    if (str[i])
        result[j++] = str[i++];
}

// After (fixed):
while (str[i])
{
    if (!in_quote && (str[i] == '"' || str[i] == '\''))
    {
        in_quote = str[i];
        i++;
        continue ;
    }
    if (in_quote && str[i] == in_quote)
    {
        in_quote = 0;
        i++;
        continue ;
    }
    result[j++] = str[i++];
}
```

### 2. KAN-85-norminette-root-files (PR #63) ✅
**Status**: All tests passing
**Test Results**: 100% success rate across all test suites

**Key Difference**: This branch has the correct `remove_quotes()` implementation that was used to fix KAN-84.

### 3. KAN-81-norminette-tokenizer (PR #52) ⚠️
**Status**: Has some test failures (documented in PR #64)
**Issue**: Different problem from KAN-84 - involves trailing spaces in quoted string output

**Root Cause Analysis** (from PR #64 documentation):
- Echo builtin verified correct (adds spaces only between args)
- Issue appears to be in quote removal or token value creation in tokenizer
- Affects only quoted strings; unquoted args work fine
- Pattern: `"text"` → `text ` instead of `text` (extra trailing space)

**Most Likely Location**: `remove_quotes()` in `src/expander/var_expand.c` (same function, different issue)
**Status**: Requires separate investigation and fix

## Comparison: Why KAN-85 Passes But KAN-84 Failed

The key difference between the passing (KAN-85) and failing (KAN-84) branches was in the expander module's quote handling:

1. **KAN-84 (failing)**: Used abstracted helper function that introduced subtle bugs
2. **KAN-85 (passing)**: Used simpler, more direct implementation

The lesson: Sometimes simpler code is better code. The attempt to abstract the quote handling into a separate function introduced complexity and bugs.

## Recommendations

### For KAN-84-norminette-expander:
✅ **RESOLVED** - Apply the fix from this PR to resolve the failing tests

### For KAN-81-norminette-tokenizer:
- Investigate the trailing space issue in quote handling
- Review tokenizer's quote removal logic
- Check if it's related to the same `remove_quotes()` function or a different part of the pipeline
- Reference the analysis in PR #64 for detailed findings

### General Best Practices:
1. Keep quote handling logic simple and direct
2. Write comprehensive tests for complex quote patterns
3. Test edge cases like adjacent quotes (`""text""`)
4. Compare implementations across branches when debugging
5. Use the working implementation as a reference when fixing bugs

## Testing Methodology

All fixes were validated with:
1. Local test execution (export, cd, echo test suites)
2. Code review for quality and style
3. Security scanning with CodeQL
4. Comparison with known-working implementations

## Files Modified
- `src/expander/get_var_value.c` - Fixed `remove_quotes()` function

## Success Metrics
- ✅ 100% test pass rate achieved (25/25 export tests)
- ✅ No regressions in other test suites
- ✅ No security vulnerabilities introduced
- ✅ Code review passed (only minor stylistic suggestions)
