/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:54:56 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/12 15:19:03 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

void	modify_env_value(t_env *cur, char * limit)
{
	free(cur->value);
	cur->value = ft_strdup(limit + 1);
}

void	create_new_env_node(char *to_compare, t_env *cur, char *limit, char *export)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return;
	new->content = strdup(export);
	new->name = ft_strdup(to_compare);
	new->value = ft_strdup(limit + 1);
	ft_env_add_back(&cur, new);
}

void	ft_export(char	**export, t_shell *shell)
{
	t_env	*cur;
	int		i;
	char	*limit;
	int		len;
	char	*to_compare;
	bool	exist;

	(void)export;

	i = 1;
	len = 0;
	cur = shell->env;
	while (export[i])
	{
		// if (!ft_strrchr(export[i], '='))
		// {// s'il y a export VAR et pas de = : on stock la variable en attendant une value;
		// 	shell->hidden = malloc(sizeof(t_env));
		// 	shell->hidden->name = ft_strdup(ft_strrchr(export[i], '='));
		// 	//il faut que je add back avec une cpy (voir mes fonctions d'env)
		// 	shell->hidden = shell->hidden->next;
		// }
		exist = false;
		if (ft_strchr(export[i], '=') && exist == false)
		{
			limit = ft_strchr(export[i], '=');
			len = limit - export[i];
			to_compare = ft_strndup(export[i], len);
			while (cur != NULL && cur->next != NULL && export[i])
			{
				if (ft_strcmp(to_compare, cur->name) == 0)
				{
					modify_env_value(cur, limit);
					exist = true;
					break;
				}
				else if (ft_strcmp(to_compare, cur->name) != 0 && exist == false)
					create_new_env_node(to_compare, cur, limit, export[i]);
				cur = cur->next;
			}
			free(to_compare);
		}
	// 	if (limit != NULL)
	// 	{
	// 		while (cur != NULL)
	// 		{
	// 			// limit = ft_strchr(cur->content, '=');
	// 			// if (limit)
	// 			// {
	// 			// 	len = limit - export[i];
	// 			// 	free(cur->value);
	// 			// 	cur->value = ft_strdup(limit + 1);
	// 			// }
	// 			cur = cur->next;
	// 		}
	// 	}
		i++;
	}
	// sort_env_list(cur, shell);
	cur = shell->env;
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

