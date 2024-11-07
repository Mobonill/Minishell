/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:54:56 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/07 18:22:32 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

void	ft_export(char	**export, t_shell *shell)
{
	t_env	*cur;
	t_env	*tmp;

	cur = shell->env;
	while (cur != NULL)
	{
		if (ft_strncmp(cur->name, cur->next->name, 1) > 0)
		{
			tmp = cur->next->name;
			cur->next->name = cur->name;
			cur->name = tmp;
		}
		cur = cur->next;
	}
}

void	add_new_env_variable(char **export, t_shell * shell)
{

}

void	modify_env_variable(char **export, t_shell *shell)
{

}

