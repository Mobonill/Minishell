/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:33 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/03 15:48:19 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// TO COMPILE : gcc lexer.c -lreadline


#ifndef LEXER_H
# define LEXER_H

# include "libft/libft.h"
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


typedef enum token
{
	WORD,
	PIPE,
	SIMPLEQUOTE,
	DOUBLEQUOTE,
	IN_REDIR, // < 
	OUT_REDIR, // >
	DELIMITER, // <<
	APPEND, // >>
	HERE_DOC,
	SPACES,
}	token;

typedef struct s_lexer
{
	char    	*str;
	t_tokens        token;
	int		i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;


typedef struct s_exec
{
	int	exit_status;
	
}	t_exec;

typedef struct s_parser
{
	char                    **str;
	int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	int                     num_redirections;
	char                    *hd_file_name;
	t_lexer                 *redirections;
	struct s_parser	*next;
	struct s_parser	*prev;
}	t_parser;

typedef struct s_env
{
	char			*name;
	char			*value;
	char			*content;
	int				index;
	struct s_env	*next;
}					t_env;

// ENVP
void	init_envp(const char **envp);
void	get_env_names_and_values(t_env *env);
void	ft_envclear(t_env **env, void (*del)(void *));
t_env	*ft_envnew(char *content);
t_env	*ft_envlast(t_env *env);
void	ft_env_add_back(t_env **env, t_env *new);
void	print_env(t_env *env);
void	free_env(t_env *env);

// LEXER
void	minishell_loop();


#endif