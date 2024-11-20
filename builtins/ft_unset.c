/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:55:36 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/09 16:25:37 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

void	ft_unset(char **unset, t_shell *shell)
{
	int		i;
	t_env	*cur;
	t_env	*prev;

	i = 1; // j'ignore mon "unset"
	while (unset[i])
	{
		cur = shell->env; // R
		prev = NULL;
		while (cur != NULL)
		{
			if (ft_strcmp(unset[i], cur->name) == 0)
			{
				if (prev == NULL)
					cur = cur->next; // shell->env = tmp->next;
				else
					prev->next = cur->next; // prev->next =  shell->env->next
				free(cur->name);
				free(cur->value);
				free(cur->content);
				free(cur);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
		i++;
	}
}
