/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:33 by mobonill          #+#    #+#             */
/*   Updated: 2024/10/28 17:29:36 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// TO COMPILE : gcc lexer.c -lreadline


#ifndef LEXER_H
# define LEXER_H

# include "../libft/libft.h"
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/wait.h>
# include <errno.h>
# include <pthread.h>
# include <sys/time.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

// echo "Bonjour" | echo "Salut" > file.txt

// LEXER ---> [echo] --> [Bonjour] ---> [ | ] ---> [echo] ---> [Salut] ---> [ > ] ----> [file.txt]
//				0	 next	 0		next  1			  0			  0			  2				0

typedef enum token
{
	WORD,
	PIPE,
	SIMPLEQUOTE,
	DOUBLEQUOTE,
	IN_REDIR, // < 
	OUT_REDIR, // >
	HERE_DOC,
	SPACES,
} token ;

typedef struct s_lexer
{
	char	*str;
	int		token;

}	t_lexer;

void	minishell_loop();


#endif