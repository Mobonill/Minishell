/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:54:56 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/08 18:31:38 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

void	ft_export(char	**export, t_shell *shell)
{
	t_env	*cur;
	int		i;
	char	*limit;
	int		len;

	i = 1;
	len = 0;
	cur = shell->env;
	printf("%s\n", ft_strchr(export[i], '='));
	while (export[i])
	{
		limit = ft_strchr(export[i], '=');
		if (limit != NULL)
		{
			while (cur != NULL)
			{
				limit = ft_strchr(cur->content, '=');
				if (limit)
				{
					len = limit - export[i];

					free(cur->value);
					cur->value = ft_strdup(limit + 1);
				}
			}
		}
	}
	// sort_env_list(cur, shell);
	while (cur != NULL)
	{
		printf("%s",cur->name);
		printf("=");
		printf("%s\n", cur->value);
		cur = cur->next;
	}

}
void	swap_env(t_env *cur)
{
	char	*tmp_name;
	char	*tmp_value;

	tmp_name = cur->next->name;
	tmp_value = cur->next->value;
	cur->next->name = cur->name;
	cur->next->value = cur->value;
	cur->name = tmp_name;
	cur->value = tmp_value;
}
void	sort_env_list(t_env *cur, t_shell *shell)
{
	t_env	*last;
	bool	check;
	
	cur = shell->env;
	last = NULL;
	check = false;
	while (cur != last && cur->next != last)
	{
		if (check == true)
		{
			cur = shell->env;
			check = false;
		}
		if (ft_strcmp(cur->name, cur->next->name) > 0)
		{
			swap_env(cur); 
			cur = shell->env;
			check= true;
		}
		// if (myListIsSorted(shell) == true)
		// 	break;
		cur = cur->next;
	}
	last = cur;
}

// bool	myListIsSorted(t_shell *shell)
// {
// 	t_env	*cur;
// 	bool		sorted;

// 	cur = shell->env;
// 	sorted = true;
// 	while (cur->next != NULL)
// 	{
// 		if (ft_strcmp(cur->name, cur->next->name) > 0) {
// 			return (false);
// 		}
// 		cur = cur->next;
// 	}
// 	return (sorted);
// }

// void	add_new_env_variable(char **export, t_shell * shell)
// {

// }

// void	modify_env_variable(char **export, t_shell *shell)
// {

// }

