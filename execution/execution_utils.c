/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:21:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/23 18:56:23 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	closing_child_pipes(t_exec *exec)
{
	int	j;

	j = -1;
	while (++j < exec->num_pipes)
	{
		close(exec->fd[j][0]);
		close(exec->fd[j][1]);
	}
}
int	manage_dup(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0)
		return (errno);
	return (0);
}
