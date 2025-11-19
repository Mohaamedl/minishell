# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: framiran <framiran@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 17:18:03 by framiran          #+#    #+#              #
#    Updated: 2025/04/29 18:33:53 by framiran         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = libft.a

INCLUDES = includes

SRCS = ft_bzero.c ft_strlen.c ft_atoi.c ft_isalpha.c ft_isalnum.c ft_isascii.c \
       ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
       ft_memmove.c ft_memset.c ft_strchr.c ft_strlcpy.c ft_calloc.c ft_strdup.c \
       ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_strlcat.c \
       ft_substr.c ft_strjoin.c ft_split.c ft_strtrim.c ft_itoa.c ft_strmapi.c \
       ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_putendl_fd.c

BONUS_SRCS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
             ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
             ft_lstmap.c


OBJFILES = $(SRCS:.c=.o)
BONUS_OBJFILES = $(BONUS_SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDES)

all: $(NAME)

$(NAME): $(OBJFILES)
	ar rcs $(NAME) $(OBJFILES)

bonus: $(OBJFILES) $(BONUS_OBJFILES)
	ar rcs $(NAME) $(OBJFILES) $(BONUS_OBJFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES) $(BONUS_OBJFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all
