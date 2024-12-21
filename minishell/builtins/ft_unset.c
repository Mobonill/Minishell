/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:55:36 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:36:16 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_unset_node(char *name, t_shell *shell)
{
	t_env	*cur;
	t_env	*prev;

	cur = shell->env;
	prev = NULL;
	while (cur != NULL)
	{
		if (ft_strcmp(name, cur->name) == 0)
		{
			if (prev == NULL)
				shell->env = cur->next;
			else
				prev->next = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur->content);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	ft_unset(char **unset, t_shell *shell)
{
	int	i;

	i = 1;
	g_global_exit = 0;
	while (unset[i])
	{
		ft_unset_node(unset[i], shell);
		i++;
	}
}
