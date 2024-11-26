/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/26 18:59:07 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**transform_env_list_to_tab(t_shell *shell, t_exec *exec)
{
	t_env	*cur;
	int		i;
	int		size_env;
	
	cur = shell->env;
	i = 0;
	size_env = ft_envsize_minishell(shell->env);
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	if (!exec->env)
		return NULL;
	exec->env[size_env] = NULL;
	while (cur)
	{
		exec->env[i] = ft_strdup(cur->content);
		cur = cur->next;
		i++;
	}
	return (exec->env);
}

int	execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec	*exec;
	int		i;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	exec->path = find_path(parser, shell);
	exec->num_pipes = ft_lstsize_minishell(parser) - 1;
	exec->pid = malloc(sizeof(pid_t) * (exec->num_pipes + 1));
	exec->fd = malloc(sizeof(int *) * (exec->num_pipes + 3));
	if (!exec->fd || !exec->pid)
	{
		perror("");
		return (errno);
	}
	i = -1;
	while (++i < exec->num_pipes)
	{
		exec->fd[i] = malloc(sizeof(int) * 2);
		if (!exec->fd[i] || pipe(exec->fd[i]) != 0)
		{
			return (errno);
		}
	}
	fork_system_call(parser, exec, shell);
	return (0);
}
void	fork_system_call(t_simple_cmds *parser, t_exec *exec, t_shell *shell)
{
	int	i;

	i = -1;
	while (++i <= parser->num_redirections)
	{
		exec->pid[i] = fork();
		printf("pid i %d\n", exec->pid[i]);
		if (exec->pid[i] < 0)
		{
			perror("");
			// free_all(exec);
			exit(errno);
		}
		else if (exec->pid[i] == 0)
		{
			child_process(exec, parser, i, shell);
		}
	}
	parent_process(exec);
}

int	child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	printf("O OOOOOOOOOOOOOOOOOOOO.|.OOOOOOOOOOOOOOOOOOOOO O");
	if (handle_redirections(exec, parser) < 0)
		exit(1);
	if (i == 0)
	{
		if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("FIRST dup2 failed");
			exit(1);
		}
		close(exec->fd[i][1]);
	}
	else if (i == exec->num_pipes)
	{
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("END dup2 failed");
			exit(1);
		}
		close(exec->fd[i - 1][0]);
	}
	else
	{
		if (dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0 || dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("MID dup2 failed\n");
			exit(1);
		}
		close(exec->fd[i - 1][0]);
		close(exec->fd[i][1]);
	}
	closing_child_pipes(exec);
	execute_command(parser, shell, exec);
	exit(0);
}

void	execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	char	*cmd_path;
	int		i;
	int		size_env;

	i = 0;
	size_env = ft_envsize_minishell(shell->env);
	cmd_path = find_path(parser, shell);
	if (!cmd_path)
	{
		ft_fprintf(2, "%s: command not found\n", parser->str[0]);
		free_cmd_argv(parser);
		free_all(exec);
		exit(127);
	}
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	exec->env[size_env] = NULL;
	if (!exec->env)
		return ;
	transform_env_list_to_tab(shell, exec);
	for (i = 0; printf("%s\n", exec->env[i]); i++);
	if (execve(cmd_path, parser->str, exec->env) == -1)
	{
		perror(parser->str[0]);
		free(cmd_path);
		free_cmd_argv(parser);
		free_all(exec);
		exit(126);
	}
	free(cmd_path);
	free_cmd_argv(parser);
}

void	parent_process(t_exec *exec)
{
	int	i;
	int	final_status;

	i = 0;
	final_status = 0;
	while (i < exec->num_pipes)
	{
		close(exec->fd[i][0]);
		close(exec->fd[i][1]);
		++i;
	}
	i = 0;
	while (i <= exec->num_pipes)
	{
		waitpid(exec->pid[i], &exec->status, 0);
		if (WIFEXITED(exec->status))
			final_status = WEXITSTATUS(exec->status);
		else if (WIFSIGNALED(exec->status))
			final_status = 128 + WTERMSIG(exec->status);
		i++;
	}
	// free_all(exec);
	exit(final_status);
}


// int	my_choosen_exec(char *str, t_shell *shell, t_parser *parser)
// {
// 	char *unset[] = {
// 		"UNSET" ,
// 		"1pager" ,
// 		"_less",
// 		"PWD",
// 		NULL
// 	};

// 	char *export[] = {
// 		"EXPORT" ,
// 		"LESS=2TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT1",
// 		"ttttttttttttttttt=tttttttttttt" ,
// 		"452ffffffffffffffffffffff",
// 		"_ttttttttttttttttttt"
// 		"-ffffffffffffffffffffffff"
// 		"tttttttttt754t",
// 		"t=454ttttttttttttttttttttt",
// 		NULL
// 	};
// 	t_parser *cur;

// 	cur = NULL;
// 	while (cur != NULL)
// 	{
// 		if (cur->redirections->str == HERE_DOC)
// 		{
// 			ft_handle_heredoc(parser);
// 		}
// 		cur = cur->next;
// 	}

	
// 	// si on a un shell->hidden, on strcmp(cdm, hidden) et si c'est == et qu'il y a un = juste apres, on entre une value et on add le node.
// 	// // if (shell->hidden)
// 	// // {
// 	// 		cur = 
// 	// // 		while ()
// 	// // }
// 	if (!ft_strcmp(str, "ECHO"))
// 		return 0;
// 		// return (ft_cd); ( ? =cd retourne 1 en cas d erreur dee chemin)
// 	else if (!ft_strcmp(str, "PWD"))
// 		return (ft_pwd(shell->env), 0);
// 	else if (!ft_strcmp(export[0], "EXPORT"))
// 		return (ft_export(export, shell), 0);
// 	else if (!ft_strcmp(unset[0], "UNSET"))
// 		return (ft_unset(unset, shell), 0);
// 	else if(!ft_strcmp(str, "ENV"))
// 		return (ft_env(shell->env), 0);
// 	else if (!ft_strcmp(str, "EXIT"))
// 		return 0;
// 		// return (ft_exit);
// 	else
// 		execute_minishell();
// }