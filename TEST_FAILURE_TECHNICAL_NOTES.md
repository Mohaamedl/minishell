# Technical Notes - Test Failure Investigation

## Echo Builtin Analysis

### Current Implementation (`src/builtins/echo.c`)

The echo builtin implementation appears correct at first glance:

```c
while (args[i])
{
    write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
    if (args[i + 1])  // Only add space if there's a next argument
        write(STDOUT_FILENO," ", 1);
    i++;
}
if (newline)
    write(STDOUT_FILENO,"\n", 1);
```

**Logic:** The code correctly checks `if (args[i + 1])` before adding a space, which means it should NOT add a space after the last argument.

### Problem Investigation

Since the echo builtin code appears correct, the issue must be in **how the arguments are constructed** before reaching the echo function.

## Hypothesis: The Problem is Upstream

The extra space is likely being added during one of these stages:

### 1. Tokenization Phase
**Location:** `src/tokenizer/`

When quoted strings are tokenized, they might be preserving trailing spaces:
- `"*.txt"` → Token value: `"*.txt "`

### 2. Quote Removal Phase
**Likely location:** During expansion or command preparation

When quotes are removed, the process might be adding a space:
- Token: `"*.txt"` → After quote removal: `*.txt ` (with space)

### 3. Argument Array Construction
**Location:** Command execution preparation

When building the `args[]` array passed to `builtin_echo()`, extra spaces might be added to each argument.

## Debugging Strategy

### Step 1: Verify Arguments Received by Echo

Add debug output to `builtin_echo()`:

```c
int builtin_echo(char **args)
{
    int i;
    
    // DEBUG: Print each argument with boundaries
    fprintf(stderr, "DEBUG echo args:\n");
    for (i = 0; args[i]; i++)
    {
        fprintf(stderr, "  args[%d] = '[%s]' (len=%zu)\n", 
                i, args[i], ft_strlen(args[i]));
    }
    
    // ... rest of function
}
```

**Expected output for:** `echo "*.txt"`
```
DEBUG echo args:
  args[0] = '[echo]' (len=4)
  args[1] = '[*.txt]' (len=5)
```

**If we see:**
```
  args[1] = '[*.txt ]' (len=6)  ← SPACE DETECTED
```
Then the problem is in argument construction, not echo.

### Step 2: Check Token Values

Add debug output in tokenization:

```c
// In tokenizer where tokens are created
fprintf(stderr, "TOKEN: type=%d value='[%s]' len=%zu\n",
        token->type, token->value, ft_strlen(token->value));
```

### Step 3: Check Quote Removal

If quotes are removed during expansion, verify the removal doesn't add spaces:

```c
// When removing quotes from "*.txt"
char *remove_quotes(char *str)
{
    // Check if this adds trailing space
    fprintf(stderr, "Before: '[%s]'\n", str);
    char *result = /* quote removal logic */;
    fprintf(stderr, "After:  '[%s]'\n", result);
    return result;
}
```

## Specific Files to Check

### Priority 1: Token Creation
```
src/tokenizer/create_token.c
src/tokenizer/create_token_helpers.c
src/tokenizer/handle_word_token.c
```

Look for:
- String copying that might add extra characters
- Buffer allocation with incorrect size
- Concatenation that adds spaces

### Priority 2: Quote Handling
```
src/tokenizer/handle_special_token.c
src/expander/var_expand.c
```

Look for:
- Quote removal logic
- String reconstruction after expansion
- Character-by-character copying

### Priority 3: Command Execution
```
src/executer/execute_cmd.c
src/executer/execute_cmd_helpers.c
```

Look for:
- Argument array construction
- How tokens are converted to `char **args`

## Common C String Mistakes That Could Cause This

### 1. Extra Space in String Literals
```c
// Wrong:
char *str = strdup("text ");  // Space at end

// Right:
char *str = strdup("text");
```

### 2. Incorrect Buffer Size
```c
// Wrong: Allocates extra byte and zeros it, creating space
char *buf = calloc(len + 2, 1);  // +2 instead of +1

// Right:
char *buf = malloc(len + 1);  // +1 for null terminator only
```

### 3. Incorrect String Copy
```c
// Wrong: Copies extra space
strncpy(dest, src, len + 1);  // If len doesn't account for actual length

// Right:
strncpy(dest, src, len);
dest[len] = '\0';
```

### 4. Quote Removal Adding Space
```c
// Wrong: Leaves space when removing quotes
"*.txt" → *.txt   (intention)
"*.txt" → *.txt\0 (with space before \0)

// Check if quote removal does: result[i++] = ' '; somewhere
```

## Test Commands for Local Debugging

```bash
# Build with debug output
make re

# Test cases that fail
echo "*.txt"         # Should: *.txt | Currently: *.txt 
echo '*.txt'         # Should: *.txt | Currently: *.txt 
echo "*"             # Should: *     | Currently: * 
echo "$VAR"          # Should: value | Currently: value 
echo ""              # Should: empty | Currently: space
```

## Expected vs Actual Behavior

### Test: `echo "*.txt"`

**Expected Flow:**
1. Tokenizer: Creates token with value `"*.txt"` (includes quotes)
2. Quote Removal: Removes quotes → `*.txt`
3. Wildcard Check: Detects quotes, prevents expansion
4. Arg Array: `args[0]="echo"`, `args[1]="*.txt"`
5. Echo Output: Prints `*.txt` then `\n`
6. Final: `*.txt\n`

**Actual Flow (Bug):**
1. Tokenizer: Creates token with value `"*.txt"` (includes quotes)
2. Quote Removal: Removes quotes → `*.txt ` ← **SPACE ADDED HERE?**
3. Wildcard Check: Detects quotes, prevents expansion
4. Arg Array: `args[0]="echo"`, `args[1]="*.txt "`
5. Echo Output: Prints `*.txt ` then `\n`
6. Final: `*.txt \n` ← **EXTRA SPACE**

## Likely Culprit Code Patterns

Look for these patterns in the codebase:

### Pattern 1: Space in Buffer Initialization
```c
// Check for:
char buffer[SIZE];
memset(buffer, ' ', SIZE);  // Sets everything to space!
```

### Pattern 2: Space in String Building
```c
// Check for:
result = malloc(len + 1);
strcpy(result, str);
strcat(result, " ");  // Adds space!
```

### Pattern 3: Incorrect Length Calculation
```c
// Check for:
int len = ft_strlen(str) + 1;  // +1 is for null terminator
char *new = malloc(len + 1);   // Another +1 ← BUG: too much space
strcpy(new, str);
// The extra byte might become a space
```

## Next Steps for Developer

1. **Add debug output** to track where space is added
2. **Run single test** with debug output enabled
3. **Follow the data** from tokenization to execution
4. **Identify the exact line** that adds the space
5. **Fix the issue** (likely 1-2 line change)
6. **Verify fix** with all test suites
7. **Submit PR** with fix

## Additional Context

The fact that ALL quote-related tests fail with the same pattern (trailing space) suggests a **systematic issue in quote handling**, not a random bug. This is good news because:

- ✅ Easy to reproduce
- ✅ Consistent behavior
- ✅ Likely a single root cause
- ✅ Should be a simple fix once found

## Related Test Failures

All failures follow this pattern:

| Test Category | Pattern | Count |
|--------------|---------|-------|
| Wildcard (quoted) | `"pattern"` → `pattern ` | 3 |
| Variable (quoted) | `"$VAR"` → `value ` | 3 |
| Echo (empty/special) | `""` → ` `, `"text"` → `text ` | 3 |

Total: **9 tests**, all with same symptom: trailing space.

---

**Document Version:** 1.0  
**Last Updated:** 2026-01-15  
**Status:** Investigation phase - awaiting debug output
