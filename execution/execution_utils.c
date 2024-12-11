/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:21:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/11 16:12:48 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	closing_child_pipes(t_exec *exec, int i)
// {
// 	int	j;

// 	j = -1;
// 	while (++j < exec->num_pipes)
// 	{
// 		if (j != i - 1)
// 		{
// 			printf("Pipe fd[0] number %d closed\n", j);
// 			close(exec->fd[j][0]);
// 		}
// 		if (j != i)
// 		{
// 			printf("Pipe fd[1] number %d closed\n", j);
// 			close(exec->fd[j][1]);
// 		}
// 	}
// }
void closing_child_pipes(t_exec *exec, int current_index)
{
	int j;

	j = -1;
	if (exec->output != -1)
		close(exec->output);
	if (!exec->fd)
		return;
	while (++j < exec->num_pipes)
	{
		if (j != current_index - 1)
			close(exec->fd[j][0]);
		if (j != current_index)
			close(exec->fd[j][1]);
	}
}

int	manage_dup(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0)
		return (errno);
	return (0);
}

int	ft_lstsize_minishell(t_simple_cmds *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

int	ft_envsize_minishell(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}
int is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
			!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "echo"));
}
void cleanup_and_exit(t_exec *exec, t_shell *shell)
{
	free_exec_env(exec->env);
	ft_free_tous(shell);
	exit(errno);
}
