/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:21:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/27 13:17:04 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	closing_child_pipes(t_exec *exec, int i)
{
	int	j;

	j = -1;
	while (++j < exec->num_pipes)
	{
		if (j != i - 1)
			close(exec->fd[j][0]);
		if (j != i)
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
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "export") || !strcmp(cmd, "unset") ||
			!strcmp(cmd, "env") || !strcmp(cmd, "exit") || !strcmp(cmd, "echo"));
}
