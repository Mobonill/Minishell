/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:55:57 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/05 14:07:59 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

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