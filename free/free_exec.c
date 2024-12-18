/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:22:58 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/18 18:28:11 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_exec_env(char **env)
{
	int i;

	if (!env)
		return;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}


void	free_pipes(t_exec *exec)
{
	int	i;

	if (!exec || !exec->fd)
		return;
	if (exec->fd)
	{
		i = 0;
		while (i < exec->num_pipes)
		{
			if (exec->fd[i])
			{
				free(exec->fd[i]);
				exec->fd[i] = NULL;
			}
			i++;
		}
		free(exec->fd);
		exec->fd = NULL;
	}
}

void	free_all(t_exec *exec)
{

	free_exec_env(exec->env);
	exec->env = NULL;
	free_pipes(exec);
	free(exec->pid);
}

void	free_cmd_argv(t_simple_cmds *parser)
{
	int i;

	i = 0;
	if (parser == NULL || parser->str == NULL)
		return ;
	while (parser->str[i])
	{
		if (parser->str[i] != NULL)
			free(parser->str[i]);
		i++;
	}
	free(parser->str);
	parser->str = NULL;
}