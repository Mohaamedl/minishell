# **************************************************************************** #
#                                   MINISHELL                                  #
# **************************************************************************** #

NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDES    = -Iinclude -I. -ILibft
LIBS        = -lreadline

# Directories
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = include
LIBFT_DIR   = Libft
LIBFT       = $(LIBFT_DIR)/libft.a

# Source files - Built-ins and core from src/
SRCS        = $(SRC_DIR)/main.c \
			  $(SRC_DIR)/main_helpers.c \
			  $(SRC_DIR)/init.c \
			  $(SRC_DIR)/cleanup.c \
			  $(SRC_DIR)/parser_simple.c \
			  $(SRC_DIR)/environment/env_init.c \
			  $(SRC_DIR)/environment/env_get.c \
			  $(SRC_DIR)/environment/env_set.c \
			  $(SRC_DIR)/environment/env_utils.c \
			  $(SRC_DIR)/signals/signals.c \
			  $(SRC_DIR)/signals/signals_heredoc.c \
			  $(SRC_DIR)/builtins/echo.c \
			  $(SRC_DIR)/builtins/cd.c \
			  $(SRC_DIR)/builtins/pwd.c \
			  $(SRC_DIR)/builtins/export.c \
			  $(SRC_DIR)/builtins/unset.c \
			  $(SRC_DIR)/builtins/env.c \
			  $(SRC_DIR)/builtins/exit.c \
			  $(SRC_DIR)/builtins/builtin_utils.c \
			  $(SRC_DIR)/utils/string_utils.c \
			  $(SRC_DIR)/utils/string_utils2.c \
			  $(SRC_DIR)/utils/char_utils.c \
			  $(SRC_DIR)/utils/ft_split.c \
			  $(SRC_DIR)/utils/memory_utils.c \
			  $(SRC_DIR)/utils/number_utils.c \
			  $(SRC_DIR)/utils/error.c \
			  $(SRC_DIR)/tokenizer/tokenizer.c \
			  $(SRC_DIR)/tokenizer/tokenizer_helpers.c \
			  $(SRC_DIR)/tokenizer/token_memory.c \
			  $(SRC_DIR)/tokenizer/type_handlers.c \
			  $(SRC_DIR)/tokenizer/validate_token_list.c \
			  $(SRC_DIR)/cmd_and_ops_list/build_list.c \
			  $(SRC_DIR)/cmd_and_ops_list/create_cmd_node.c \
			  $(SRC_DIR)/cmd_and_ops_list/create_cmd_node_helpers.c \
			  $(SRC_DIR)/cmd_and_ops_list/create_op_node.c \
			  $(SRC_DIR)/cmd_and_ops_list/node_memory.c \
			  $(SRC_DIR)/debuggers/print_tokens.c \
			  $(SRC_DIR)/debuggers/print_nodes.c \
			  $(SRC_DIR)/ast/build_tree.c \
			  $(SRC_DIR)/ast/build_tree_helpers.c \
			  $(SRC_DIR)/ast/ast_memory.c \
			  $(SRC_DIR)/ast/ast_debuggers.c \
			  $(SRC_DIR)/expander/var_expand.c \
			  $(SRC_DIR)/expander/wildcard.c \
			  $(SRC_DIR)/expander/wildcard_match.c \
			  $(SRC_DIR)/expander/wildcard_expand.c \
			  $(SRC_DIR)/expander/wildcard_args.c \
			  $(SRC_DIR)/execution/execute_ast.c \
			  $(SRC_DIR)/execution/pipeline_helpers.c \
			  $(SRC_DIR)/execution/pipeline_execution.c \
			  $(SRC_DIR)/execution/apply_redirections.c \
			  $(SRC_DIR)/execution/execute_cmd.c \
			  $(SRC_DIR)/execution/execute_cmd_helpers.c \
			  $(SRC_DIR)/execution/handle_redirections.c \
			  $(SRC_DIR)/execution/heredoc.c \
			  $(SRC_DIR)/execution/heredoc_utils.c \
			  $(SRC_DIR)/process/process.c \
			  $(SRC_DIR)/process/wait.c \
			  $(SRC_DIR)/process/exec.c \
			  $(SRC_DIR)/process/pipes.c \
			  $(SRC_DIR)/process/pipes_utils.c \

# Object files
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ALL_OBJS    = $(OBJS)

# Colors
GREEN       = \033[0;32m
RED         = \033[0;31m
BLUE        = \033[0;34m
YELLOW      = \033[0;33m
NC          = \033[0m

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(ALL_OBJS) $(LIBFT)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(ALL_OBJS) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(NC)"

$(LIBFT):
	@echo "$(BLUE)Building Libft...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# **************************************************************************** #
#                                   CLEANUP                                    #
# **************************************************************************** #

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(GREEN)✓ Clean complete!$(NC)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Full clean complete!$(NC)"

re: fclean all

norm:
	@echo "$(BLUE)Running norminette...$(NC)"
	@norminette $(SRC_DIR) $(INC_DIR) || true
	@norminette tokenizer ast cmd_and_ops_list debuggers main.c main_helpers.c || true

test: all
	@echo "$(BLUE)Running tests...$(NC)"
	@bash tests/run_all_tests.sh || true

.PHONY: all clean fclean re norm test
