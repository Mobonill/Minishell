/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:55:57 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/23 16:24:35 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur != NULL)
	{
		printf("%s", cur->name);
		printf("=");
		printf("%s\n", cur->value);
		cur = cur->next;
	}
}