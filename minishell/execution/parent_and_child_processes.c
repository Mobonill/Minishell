/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_and_child_processes.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:00:07 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 19:11:15 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_first_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i == 0)
	{
		if (exec->output != -1)
			dup2(exec->output, STDOUT_FILENO);
		else if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
		{
			perror("");
			return (1);
		}
	}
	return (0);
}

int	handle_middle_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i > 0 && i < exec->num_pipes)
	{
		if (!exec->num_heredoc && dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("");
			return (1);
		}
		if (exec->output != -1)
		{
			if (dup2(exec->output, STDOUT_FILENO) < 0)
			{
				perror("");
				return (1);
			}
		}
		else
		{
			if (dup2(exec->fd[i][1], STDOUT_FILENO) < 0)
			{
				perror("");
				return (1);
			}
		}
	}
	return (0);
}

int	handle_last_pipe(t_exec *exec, int i)
{
	if (exec->num_pipes > 0 && i == exec->num_pipes)
	{
		if (!exec->num_heredoc && dup2(exec->fd[i - 1][0], STDIN_FILENO) < 0)
		{
			perror("");
			return (1);
		}
		if (exec->output != -1)
		{
			if (dup2(exec->output, STDOUT_FILENO) < 0)
			{
				perror("");
				return (1);
			}
		}
	}
	return (0);
}

int	child_process(t_exec *exec, t_simple_cmds *parser, int i, t_shell *shell)
{
	if (handle_redirections_and_heredoc(exec, parser, i) < 0)
		exit(1);
	if ((!parser || !parser->str || !parser->str[0]) && exec->num_pipes == 0)
		exit(0);
	if (exec->num_pipes > 0)
	{
		if (i == 0)
			if (handle_first_pipe(exec, i) < 0)
				exit(1);
		if (i == exec->num_pipes)
			if (handle_last_pipe(exec, i) < 0)
				exit(1);
		if (i != 0 && i != exec->num_pipes)
			if (handle_middle_pipe(exec, i) < 0)
				exit(1);
		closing_child_pipes(exec, i);
	}
	if (is_builtin(parser->str[0]))
	{
		execute_builtin(parser, shell, exec);
		exit(g_global_exit);
	}
	execute_command(parser, shell, exec);
	return (0);
}

void	parent_process(t_exec *exec)
{
	int	i;

	i = 0;
	while (i < exec->num_pipes)
	{
		close(exec->fd[i][0]);
		close(exec->fd[i][1]);
		++i;
	}
}
