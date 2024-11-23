/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:22:58 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/23 16:25:07 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_pipes(t_exec *exec)
{
	int i;

	i = 0;
	if (exec->fd)
	{
		i = 0;
		while (i <= exec->num_pipes + 2) // pour les redirections
		{
			free(exec->fd[i]);
			i++;
		}
		free(exec->fd);
	}
}
void	free_all(t_exec *exec)
{
	free_pipes(exec);
	free(exec->pid);
}

void	free_cmd_argv(t_simple_cmds *parser)
{
	int i;

	i = 0;
	while (parser->str[i])
	{
		free(parser->str[i]);
		i++;
	}
	free(parser->str);
}