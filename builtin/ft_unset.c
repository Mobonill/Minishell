/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:55:36 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/05 18:53:35 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

void	ft_unset(char **unset, t_env *env)
{
	int		i;
	t_env	*cur;
	t_env	**head = NULL;
	// t_env	*prev = NULL;

	i = 1; // j'ignore mon "unset"
	while(unset[i] != NULL)
	{
		cur = env;
		*head = cur;
		printf(" i = %d\n", i);
		printf("unset[i] = %s\n", unset[i]);
		// while(cur != NULL)
		// {
		// 	if (strcmp(unset[i], cur->name) == 0)
		// 	{
		// 		if (*head == cur)
		// 			*head = cur->next;
		// 		else if (prev != NULL)
		// 			prev->next = cur->next;
		// 		free(cur);
		// 	}
		// 	cur = cur->next;
			// prev->next = cur;
		// }
		i++;
	}
}
