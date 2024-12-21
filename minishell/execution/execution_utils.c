/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:21:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:10:00 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	closing_child_pipes(t_exec *exec, int current_index)
{
	int	j;

	j = -1;
	if (exec->output != -1)
		close(exec->output);
	if (!exec->fd)
		return ;
	while (++j < exec->num_pipes)
	{
		if (j != current_index - 1)
			close(exec->fd[j][0]);
		if (j != current_index)
			close(exec->fd[j][1]);
	}
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

void	ft_envclear(t_env **env, void (*del)(void *))
{
	t_env	*temp;

	while (*env)
	{
		temp = (*env)->next;
		if (del)
			del((*env)->content);
		if ((*env)->name)
			free((*env)->name);
		if ((*env)->value)
			free((*env)->value);
		free(*env);
		*env = temp;
	}
}

int	init_and_allocate(t_exec *exec, t_simple_cmds *parser, t_shell *shell)
{
	int	a;
	int	b;

	a = 0;
	b = 0;
	a = init_exec_memory(exec, parser);
	b = allocate_pipes(exec, shell);
	return (a + b);
}

int	ft_waitpid(t_exec *exec)
{
	int	i;

	i = -1;
	while (++i <= exec->num_pipes)
	{
		waitpid(exec->pid[i], &exec->status, 0);
		if (WIFEXITED(exec->status))
			g_global_exit = WEXITSTATUS(exec->status);
		else if (WIFSIGNALED(exec->status))
			g_global_exit = 128 + WTERMSIG(exec->status);
	}
	return (g_global_exit);
}
