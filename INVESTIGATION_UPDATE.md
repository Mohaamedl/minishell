# Investigation Update - Potential Root Cause Identified

## Date: 2026-01-15
## Status: 🎯 LIKELY CAUSE FOUND

Based on analysis of the `handle_word` function mentioned in the new requirement and further code investigation, I've identified the likely source of the trailing space issue.

## Key Finding

The problem appears to be in the **tokenization phase**, specifically in the `handle_word` function at line 105-124 of `src/tokenizer/handle_word_token.c`.

### The Suspect Code

```c
int	handle_word(char *line, int *i, t_token **last_token, t_token **head)
{
	int		j;
	t_token	*token;
	char	*str;
	int		has_equals;

	has_equals = check_has_equals(line, &j);
	if (has_equals && !handle_var_assignment(line, &j))
		return (0);
	if (!has_equals)
		handle_simple_word(line, &j);    // ← Sets j to word length
	str = malloc((j + 1) * sizeof(char));
	ft_memcpy(str, line, j);             // ← Copies j bytes
	str[j] = '\0';                        // ← Null terminates at position j
	token = create_token(str, WORD, 1, 0);
	append_token(head, last_token, token);
	*i += j;
	return (1);
}
```

## Problem Analysis

### Scenario 1: When parsing input `echo "*.txt"`

**Tokenizer flow:**
1. Processes `echo` → creates WORD token with value `"echo"`
2. Skips space
3. Encounters `"` (quote) → calls `handle_quote()` instead of `handle_word()`
4. `handle_quote()` creates quoted token with quotes removed

**This path seems OK.**

### Scenario 2: When there's text AFTER quotes

The problem might occur when **`handle_word()` is called on text that follows quotes**, or when processing continues after quote handling.

Let me check the tokenizer flow more carefully:

```c
// In tokenize() function:
while (line[i])
{
    if (is_space(line[i]))
    {
        skip_spaces(&line[i], &i);  // Advances i
        continue ;
    }
    if (is_quote(line[i]) && !handle_quote(&line[i], &i, &last, &head))
        return (free_tokens(head), NULL);
    if (is_operator(&line[i]))
    {
        handle_ops_and_reds(&line[i], &i, &last, &head);
        continue ;
    }
    if (!handle_word(&line[i], &i, &last, &head))  // ← NOTE: No continue here!
        return (free_tokens(head), NULL);
}
```

## 🔍 Critical Observation

Looking at the tokenizer main loop, I notice that after `handle_quote()` returns, the flow **falls through** to check `is_operator()` and potentially `handle_word()`.

However, `handle_quote()` advances `i` past the quotes:
```c
int	handle_quote(char *line, int *i, t_token **last_node, t_token **head)
{
	int	status;

	status = create_quoted_token(last_node, head, line, *line);
	*i = *i + get_quoted_size(line, *line) + 2;  // +2 for both quotes
	return (status);
}
```

### Wait... The Real Issue Might Be Here!

After `handle_quote()` processes `"*.txt"`, it should advance `i` by:
- Size of content: 5 characters (`*.txt`)
- Plus 2 for quotes: `+ 2`
- Total: advances by 7 positions

**BUT** the control flow doesn't `continue` after the quote check! Looking again:

```c
if (is_quote(line[i]) && !handle_quote(&line[i], &i, &last, &head))
    return (free_tokens(head), NULL);
if (is_operator(&line[i]))  // ← Checks again at new position
{
    handle_ops_and_reds(&line[i], &i, &last, &head);
    continue ;
}
if (!handle_word(&line[i], &i, &last, &head))  // ← And here!
    return (free_tokens(head), NULL);
```

**Actually, after looking more carefully, the tokenizer does NOT have explicit `continue` after `handle_quote()`, which means it falls through to subsequent checks.**

But wait - that would cause issues beyond just spaces. Let me reconsider...

## 🎯 The ACTUAL Problem (Updated Theory)

After reviewing the code flow more carefully, I believe the issue is NOT in tokenization but in **how arguments are printed by echo**.

Let me trace through a complete example: `echo "*.txt"`

### Complete Flow:

1. **Tokenization:**
   - Token 1: `value="echo"`, type=WORD
   - Token 2: `value="*.txt"` (quotes included), type=WORD, is_quoted=1

2. **Parsing/Command Creation:**
   - Creates cmd with name="echo"
   - args list: ["echo", "*.txt"]

3. **Expansion** (`expand_cmd_args` in var_expand.c:218):
   ```c
   without_quotes = remove_quotes(current->value);  // "*.txt" → *.txt
   current->value = without_quotes;  // Updates arg value
   ```

4. **Argument Array Creation** (`convert_args_to_array`):
   ```c
   args[0] = "echo";
   args[1] = "*.txt";  // Should be just this
   args[2] = NULL;
   ```

5. **Echo Execution** (`builtin_echo`):
   ```c
   write(STDOUT_FILENO, args[i], ft_strlen(args[i]));  // Writes "*.txt"
   if (args[i + 1])  // args[2] is NULL, so this is FALSE
       write(STDOUT_FILENO," ", 1);  // This should NOT execute
   ```

### So Why the Extra Space?

After this detailed analysis, I believe the issue must be in one of these locations:

## 🔴 Most Likely Culprits (Ranked):

### 1. **The `remove_quotes` function** (var_expand.c:24-57)

Looking at line 33:
```c
result = malloc(ft_strlen(str) + 1);
```

This allocates exactly the right size. But what if `ft_strlen` is counting something wrong, or the copying loop is adding extra characters?

### 2. **Memory Allocation in Token Creation**

When the token is initially created:
```c
str = malloc((size + 1) * sizeof(char));  // Line 59 in create_token_helpers.c
ft_memcpy(str, ++line, size);             // Line 60
str[size] = '\0';                          // Line 61
```

What if `size` is calculated with an extra character?

### 3. **The `ft_strlen` or `ft_memcpy` implementations**

If these Libft functions have bugs, they could add extra characters.

## 🧪 Recommended Next Step

Add debug output to `remove_quotes` function to see exactly what's being processed:

```c
static char	*remove_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	char	in_quote;

	if (!str)
		return (NULL);
	
	// DEBUG
	fprintf(stderr, "remove_quotes INPUT: '[%s]' len=%zu\n", str, ft_strlen(str));
	
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	while (str[i])
	{
		// ... existing logic ...
		result[j++] = str[i++];
	}
	result[j] = '\0';
	
	// DEBUG
	fprintf(stderr, "remove_quotes OUTPUT: '[%s]' len=%zu\n", result, ft_strlen(result));
	
	return (result);
}
```

This will show if a space is being added during quote removal.

## 💡 Alternative Theory

Could the issue be that tokens are being created with spaces from the original input line?

For example, if the input is:
```
echo "*.txt" 
        ↑
        trailing space in input
```

And the tokenizer doesn't properly trim or the token includes the trailing space from the input somehow?

---

**Conclusion:** The most likely culprit is either:
1. The `remove_quotes()` function adding a space during processing
2. The tokenization capturing extra characters beyond what it should
3. A bug in `ft_strlen()` or `ft_memcpy()` from Libft

**Action Required:** Add debug output to track the exact values through the pipeline and identify where the space appears.

---

**Updated:** 2026-01-15  
**Status:** Hypothesis formed, debugging strategy defined  
**Next:** Developer should add debug output and run local tests
