# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moh <moh@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/08 10:00:00 by moh               #+#    #+#              #
#    Updated: 2025/11/08 10:00:00 by moh              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I./include
LIBS		= -lreadline

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
INC_DIR		= include

# Source files
SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/init.c \
			  $(SRC_DIR)/cleanup.c \
			  $(SRC_DIR)/environment/env_init.c \
			  $(SRC_DIR)/environment/env_get.c \
			  $(SRC_DIR)/environment/env_set.c \
			  $(SRC_DIR)/environment/env_utils.c \
			  $(SRC_DIR)/signals/signals.c \
			  $(SRC_DIR)/utils/string_utils.c \
			  $(SRC_DIR)/utils/string_utils2.c \
			  $(SRC_DIR)/utils/char_utils.c \
			  $(SRC_DIR)/utils/ft_split.c \
			  $(SRC_DIR)/utils/memory_utils.c \
			  $(SRC_DIR)/utils/number_utils.c \
			  $(SRC_DIR)/utils/error.c

# Object files
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
NC			= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)✓ Clean complete!$(NC)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean complete!$(NC)"

re: fclean all

norm:
	@echo "$(BLUE)Running norminette...$(NC)"
	@norminette $(SRC_DIR) $(INC_DIR) || true

test: all
	@echo "$(BLUE)Running Phase 1 tests...$(NC)"
	@bash tests/phase1/test_all.sh

.PHONY: all clean fclean re norm test
