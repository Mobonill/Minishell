/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:11:47 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_exec_memory(t_exec *exec, t_simple_cmds *parser)
{
	exec->env = NULL;
	exec->num_pipes = ft_lstsize_minishell(parser) - 1;
	exec->pid = malloc(sizeof(pid_t) * (exec->num_pipes + 1));
	exec->fd = malloc(sizeof(int *) * (exec->num_pipes));
	exec->input = -1;
	exec->output = -1;
	exec->num_heredoc = 0;
	if (!exec->fd || !exec->pid)
	{
		perror("");
		free(exec->fd);
		free(exec->pid);
		free(exec);
		g_global_exit = errno;
		return (g_global_exit);
	}
	return (0);
}

int	allocate_pipes(t_exec *exec, t_shell *shell)
{
	int	i;

	i = -1;
	while (++i < exec->num_pipes)
	{
		if (exec->fd)
		{
			exec->fd[i] = malloc(sizeof(int) * 2);
			if (!exec->fd[i] || pipe(exec->fd[i]) != 0)
			{
				while (--i >= 0)
					free(exec->fd[i]);
				free(exec->fd);
				free(exec->pid);
				cleanup_and_exit(exec, shell);
				g_global_exit = errno;
				return (g_global_exit);
			}
		}
	}
	return (0);
}

void	cleanup_exec_resources(t_exec *exec)
{
	int	i;

	i = -1;
	free(exec->pid);
	if (exec->fd)
	{
		while (++i < exec->num_pipes)
			free(exec->fd[i]);
		free(exec->fd);
	}
	free(exec);
}

int	execute_minishell(t_shell *shell, t_simple_cmds *parser)
{
	t_exec	*exec;
	int		result;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (errno);
	if (init_exec_memory(exec, parser) != 0)
		return (1);
	if (parser->str && ft_lstsize_minishell(parser) == 1
		&& is_builtin(parser->str[0]))
	{
		result = execute_builtin(parser, shell, exec);
		cleanup_exec_resources(exec);
		return (result);
	}
	if (allocate_pipes(exec, shell) != 0)
		return (1);
	fork_system_call(parser, exec, shell);
	g_global_exit = ft_waitpid(exec);
	cleanup_exec_resources(exec);
	return (g_global_exit);
}

void	fork_system_call(t_simple_cmds *parser, t_exec *exec, t_shell *shell)
{
	int				i;
	t_simple_cmds	*cur;

	i = -1;
	cur = parser;
	while (++i <= exec->num_pipes)
	{
		exec->pid[i] = fork();
		if (exec->pid[i] < 0)
		{
			perror("");
			free_all(exec);
			exit(errno);
		}
		else if (exec->pid[i] == 0)
			child_process(exec, cur, i, shell);
		cur = cur->next;
	}
	parent_process(exec);
}
