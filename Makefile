# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 17:35:37 by mobonill          #+#    #+#              #
#    Updated: 2024/11/19 17:30:13 by mobonill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. #-g3 -fsanitize=address 
LREAD = -lreadline
SRC =	execution.c \
		open_and_free.c \
		# envp.c \
		# lexer.c \
		# path.c \
		# builtin/ft_env.c \
		# builtin/ft_pwd.c \
		# builtin/ft_export.c \
		# builtin/ft_unset.c \
		

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