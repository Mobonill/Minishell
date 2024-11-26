/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:35:18 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:35:21 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <ctype.h>

extern int	g_global_exit;

typedef enum s_tokens
{
	WORD,
	PIPE,
	IN,//<
	HEREDOC,// <<
	OUT,//>
	APPEND,//>>
}	t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_simple_cmds
{
	char					**str;
	int						num_redirections;
	char					*hd_file_name;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

typedef struct s_pars_mini
{
	t_lexer			*lexer_list;
	t_lexer			*redirections;
	int				num_redirections;
	char			**str;
	struct s_shell	*shell;
}	t_pars_mini;

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
	char			*input_line;
	int				count_pipe;
	t_lexer			*lexer_list;
	t_simple_cmds	*pars;
	t_pars_mini		*pars_mini;
	char			**envp;
	t_simple_cmds	*commands;
	t_env			*env;
	t_env			*hidden;
}	t_shell;

typedef struct s_temp
{
	int			i;
	int			j;
}	t_temp;


typedef struct s_exec
{
	int		input;
	int		output;
	int		**fd;
	int		status;
	int		num_pipes;
	char	**env;
	char	*path;
	char	*heredoc;
	pid_t	*pid;
}			t_exec;

void			ft_free_lex(t_shell *shell);
void			ft_free_pars(t_shell *shell);
void			free_pars(t_simple_cmds *pars);
void			free_lexer_list(t_lexer **lexer);
void			free_redirections(t_lexer *redirection);
void			free_simple_cmds_list(t_simple_cmds *commands);
void			free_command(char **cmd);
char			*ft_strjoin_free(char *s1, char *s2);
void			ft_free_tous(t_shell *shell);

int				ft_minicheck(char *line);
int				find_matching_quote(char *line, int start, char quote);
int				check_quotes(char *line);
int				ft_check_errors(t_lexer *list);

t_lexer			*lexer_create(char *value, t_tokens token, int index);
void			lexer_add_back(t_lexer **list, t_lexer *new_token);
void			ft_create_lexer_list(char *value, t_tokens type, int index,
					t_lexer **list);
int				ft_read_word(char *line);
int				ft_read_word_quote(char *line, char quote);
int				ft_read_token_1(char *line, t_lexer **list, int index);
void			ft_read_token(char *line, t_lexer **list, t_shell *shell);
char			*process_quoted_or_unquoted(char **line, int *total_len);

int				special_tokens(char *line, t_lexer **list, int *index,
					t_shell *shell);
int				create_token(char *value, int type, t_lexer **list, int *index);
int				ft_process1(char **line);
char			*ft_process2(char *result, char *temp);

void			print_tokens(t_lexer *list);
void			print_simple_cmds(t_simple_cmds *cmds);
int				ft_ifspace(char line);
size_t			ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
// char			*ft_substr(const char *str, size_t start, size_t len);
char			*ft_strdup(const char *s);
// char			*ft_iPARSERtoa(int n);

t_shell			*ft_init_shell(char **envp);
void			ft_start_loop(char **envp);

void			delete_lexer(t_lexer **lexer, int index);
int				add_new_redir(t_lexer *current, t_lexer **lexer,
					t_pars_mini *pars_mini);
int				separe_redirections(t_lexer **lexer, t_pars_mini *pars_mini);
int				count_cmd(t_lexer *lexer);
char			**create_command(t_lexer *lexer);
t_simple_cmds	*create_and_add_command(t_lexer *tmp,
					t_pars_mini *pars_mini, t_simple_cmds **commands);
t_simple_cmds	*new_simple_cmd(t_lexer *lexer, t_pars_mini *pars_mini);
t_simple_cmds	*last_simple_cmd(t_simple_cmds *list);
void			add_new_simple_cmd(t_simple_cmds **list, t_simple_cmds *new);
void			parser_part(int count_pipe, t_lexer *lexer_list,
					t_shell *shell);
void			expand_part(t_shell *shell);
char			*process_str(const char *input, t_shell *shell);
char			*allocate_result(int len);
int				ft_trouve_len(const char *input, char **envp);
char			*get_env_value(const char *var_name, char **envp);
void			handle_dollar(t_shell *shell, const char *str,
					t_temp *temp, char *result);
void			handle_quote(const char *input, int *i, char *quote, int *len);
void			han_quote1(const char *input, char *quote,
					t_temp *temp, char *result);
void			handle_variable(const char *input, int *i, int *len,
					char **envp);
void			handle_env_variable(const char *input, int *i,
					int *len, char **envp);
void			han_env1(t_shell *shell, const char *str, t_temp *temp,
					char *result);
void			handle_exit_status(char *result, t_temp *temp);
void			ft_signal_ctr_d(void);
void			ft_signal_ctr_c_hd(int status);
void			ft_signal_ctr_c(int status);
void			signals_heredoc(void);
void			signals(void);


// BUILTINS
//			env
void	ft_env(t_env *env);

//			export
void	modify_env_value(t_env *cur, char * limit);
void	create_new_env_node(char *to_compare, t_env *cur, char *limit, char *export);
void	ft_export(char	**export, t_shell *shell);
void	swap_env(t_env *cur);
void	sort_env_list(t_env *cur, t_shell *shell);

//			pwd
void	ft_pwd(t_env *env);

//			unset
void	ft_unset(char **unset, t_shell *shell);


// ENVP
t_env	*init_env(const char **envp, t_shell *shell);
void	get_env_names_and_values(t_env *env);
void	ft_envclear(t_env **env, void (*del)(void *));
t_env	*ft_envnew(char *content);
t_env	*ft_envlast(t_env *env);
void	ft_env_add_back(t_env **env, t_env *new);
void	print_env(t_env *env);
void	free_env(t_env *env);


// EXUCUTIONS UTILS
void	closing_child_pipes(t_exec *exec);
int		manage_dup(int oldfd, int newfd);
int		ft_lstsize_minishell(t_simple_cmds *lst);
int		ft_envsize_minishell(t_env *lst);


// EXECUTION
int		execute_minishell(t_shell *shell, t_simple_cmds *parser);
char	**transform_env_list_to_tab(t_shell *shell, t_exec *exec);
void	fork_system_call(t_simple_cmds *parser, t_exec *exec, t_shell *shell);
int		child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell);
void	execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec);
void	parent_process(t_exec *exec);

// PATH
char	*find_path(t_simple_cmds *parser, t_shell *shell);
void	free_path(char **path);
char	*find_command_in_path(char *cmd, char **path);
char	*get_envp_path(t_env *env);

// REDIRECTIONS AND HEREDOC
int		handle_redirections(t_exec *exec, t_simple_cmds *parser);
int		ft_handle_heredoc(t_simple_cmds *parser);

// SIGNALS
void	signal_heredoc(int sig);
void	heredoc_signals(void);

//FREE
void	free_pipes(t_exec *exec);
void	free_all(t_exec *exec);
void	free_cmd_argv(t_simple_cmds *parser);

#endif