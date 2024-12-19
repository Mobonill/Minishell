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


/*void	ft_env(t_env *env)
{
	t_env	*cur;

	if (env == NULL)
	{
		printf("bash: env: No environment variables set.\n");
		g_global_exit = 1;
		return;
	}

	cur = env;
	while (cur != NULL)
	{
		if (cur->name && cur->value)
		{
			printf("%s=%s\n", cur->name, cur->value);
		}
		else if (cur->name)
		{
			printf("%s=\n", cur->name);
		}
		cur = cur->next;
	}
	g_global_exit = 0;
}
*/