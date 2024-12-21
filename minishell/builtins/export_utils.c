/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:53:25 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:36:50 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	modify_env_value(t_env *cur, char *limit)
{
	if (limit != NULL)
	{
		if (cur->value)
			free(cur->value);
		cur->value = ft_strdup(limit + 1);
		if (!cur->value)
			return ;
	}
}

void	initialize_env_node(t_env *new_node, char *export)
{
	new_node->name = NULL;
	new_node->value = NULL;
	new_node->content = NULL;
	new_node->next = NULL;
	new_node->content = ft_strdup(export);
	if (!new_node->content)
	{
		free(new_node);
		return ;
	}
}

void	print_sorted_env(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->name);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

void	swap_env(t_env *cur)
{
	char	*tmp_name;
	char	*tmp_value;

	if (cur && cur->next)
	{
		tmp_name = cur->next->name;
		tmp_value = cur->next->value;
		cur->next->name = cur->name;
		cur->next->value = cur->value;
		cur->name = tmp_name;
		cur->value = tmp_value;
	}
}

void	sort_env_list(t_env *env)
{
	t_env	*last;
	t_env	*current;
	bool	swapped;

	last = NULL;
	while (env)
	{
		swapped = false;
		current = env;
		while (current && current->next != last)
		{
			if (current && current->next && ft_strcmp(current->name,
					current->next->name) > 0)
			{
				swap_env(current);
				swapped = true;
			}
			current = current->next;
		}
		last = current;
		if (!swapped)
			break ;
	}
}
