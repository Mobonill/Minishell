# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 17:35:37 by mobonill          #+#    #+#              #
#    Updated: 2024/10/25 17:35:50 by mobonill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I../libft 
LDFLAGS =  -Llibft  -lft

SRC = signals.c parser.c parser_trouve_len.c parser_redirection.c \
		parser_expand.c parser_dollar.c parser_create_cmd.c \
		lexer_read_token.c lexer_create.c lexer_check_errors.c \
		free.c free1.c lexer.c lexer_utils1.c lexer_utils.c \
		lexer_create2.c envp1.c cd.c echo.c exit.c

RM = rm -f
OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all : $(NAME)

$(NAME): $(OBJ) libft/libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS) $(LDFLAGS) -lreadline

libft/libft.a :
	$(MAKE) -C libft

clean:
	$(RM) $(OBJ)
	$(MAKE) clean -C libft
fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C libft
re: fclean all

.PHONY: all clean fclean re
