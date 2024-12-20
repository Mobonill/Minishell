# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 17:35:37 by mobonill          #+#    #+#              #
#    Updated: 2024/12/18 18:23:45 by mobonill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g3 #-fsanitize=address 
LREAD = -lreadline
SRC =	main.c \
		envp.c \
		lexer/lexer.c \
		lexer/lexer_utils.c \
		lexer/lexer_read_token.c \
		lexer/lexer_check_errors.c \
		lexer/lexer_create.c \
		lexer/lexer_create2.c \
		parser/parser.c \
		parser/parser_create_cmd.c \
		parser/parser_dollar.c \
		parser/parser_expand.c \
		parser/parser_redirection.c \
		parser/parser_trouve_len.c \
		parser/signals.c \
		free/free.c \
		free/free1.c \
		free/free_exec.c \
		execution/execution.c \
		execution/redirections_and_heredoc.c \
		execution/execution_utils.c \
		execution/signals_heredoc.c \
		execution/heredoc_utils.c \
		execution/path.c \
		builtins/ft_env.c \
		builtins/ft_unset.c \
		builtins/ft_pwd.c \
		builtins/ft_echo.c \
		builtins/ft_cd.c \
		builtins/ft_exit.c \
		builtins/ft_export.c \


RM = rm -f

OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all : $(NAME)

$(NAME): $(OBJ) libft/libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -Llibft -lft $(LREAD)
	
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