# **************************************************************************** #
#                                   MINISHELL                                  #
# **************************************************************************** #

# Nome do executável final
NAME        = minishell

# Compilador e flags
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -I. -ILibft
LDFLAGS     = -lreadline

# Diretório e ficheiro da Libft
LIBFT_DIR   = Libft
LIBFT       = $(LIBFT_DIR)/libft.a

# Ficheiros fonte e objetos

SRCS =  main.c \
		tokenizer/tokenizer.c \
		tokenizer/tokenizer_helpers.c \
		tokenizer/token_memory.c \
		tokenizer/type_handlers.c \
		cmd_and_ops_list/build_list.c \
		cmd_and_ops_list/create_cmd_node.c \
		cmd_and_ops_list/create_cmd_node_helpers.c \
		cmd_and_ops_list/create_op_node.c \
		cmd_and_ops_list/node_memory.c \
		debuggers/print_tokens.c \
		debuggers/print_nodes.c \

OBJS = $(SRCS:.c=.o)


# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

# Target padrão
all: $(NAME)

# Compila o executável final
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS)

# Compila a Libft (se ainda não estiver compilada)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Regra genérica: .c → .o
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# **************************************************************************** #
#                                   CLEANUP                                    #
# **************************************************************************** #

# Apaga ficheiros objeto (.o)
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean      # limpa objetos da Libft também

# Apaga tudo: objetos e executável
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean    # limpa completamente a Libft

# Limpa tudo e recompila
re: fclean all

# **************************************************************************** #
#                                   EXTRAS                                     #
# **************************************************************************** #

.PHONY: all clean fclean re
# .PHONY evita conflitos com ficheiros com esses nomes
