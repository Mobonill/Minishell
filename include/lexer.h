/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:33 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/19 18:21:05 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../libft/libft.h"
# include <limits.h>
# include <fcntl.h>
# include <stdbool.h>
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


typedef enum s_token
{
	WORD,
	PIPE,
	IN,    //<
	HEREDOC, // <<
	OUT, //>
	APPEND, //>>
}	t_token;

typedef struct s_lexer
{
	char			*str;
	t_token			token;
	int				i;
	struct s_lexer	*next;
}					t_lexer;


typedef struct s_simple_cmds
{
	char					**str;
	int						num_redirections;
	char					*hd_file_name;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

typedef struct s_env
{
	char			*name;
	char			*value;
	char			*content;
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env	*env;
	t_env	*hidden;
}			t_shell ;

typedef struct s_exec
{
	int		input;
	int		output;
	int		**fd;
	int		num_pipes;
	char	**env;
	char	*path;
	char	*heredoc;
	pid_t	*pid;
}			t_exec;


// ENVP
t_shell	*init_env(const char **envp, t_shell *shell);
void	get_env_names_and_values(t_env *env);
void	ft_envclear(t_env **env, void (*del)(void *));
t_env	*ft_envnew(char *content);
t_env	*ft_envlast(t_env *env);
void	ft_env_add_back(t_env **env, t_env *new);
void	print_env(t_env *env);
void	free_env(t_env *env);

// BUILTIN(tmp_fd)
//BUILTIN UTILS
void	swap_env(t_env *cur);
void	sort_env_list(t_env *cur, t_shell *shell);

// EXEC
int		my_choosen_exec(char *str, t_shell *shell);
int		ft_handle_heredoc(t_simple_cmds *parser);
void	execute_minishell(t_shell *shell, t_simple_cmds *parser);
void	fork_system_call(t_simple_cmds *parser, t_exec *exec);
int		manage_dup(int oldfd, int newfd);


// LEXER
void	minishell_loop(t_shell *shell);


#endif