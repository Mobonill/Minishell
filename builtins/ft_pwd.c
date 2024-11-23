/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:33:04 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/23 16:24:43 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->name, "PWD") == 0)
			printf("%s\n", cur->value);
		cur = cur->next;
	}
	return ;
}