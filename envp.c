/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:45:44 by mobonill          #+#    #+#             */
/*   Updated: 2024/10/29 23:46:17 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_env(t_env *env)
{

	while (env->lst != NULL)
	{
		free(env->name);
		free(env->value);
		ft_lstclear(&env->lst, free);
	}
	free(env);
}

// void	print_env(t_env *env)
// {
// 	t_list *curr = env->lst;

// 	while (curr != NULL)
// 	{
// 		printf("%s\n", (char *)curr->content);
// 		curr = curr->next;
// 	}
// }

void	init_env(const char **envp)
{
	int		len;
	t_env	*env;
	int		i;
	t_list	*new;

	len = 0;
	i = 0;
	env = malloc(sizeof(t_env));
	if (!env)
		return;
	env->lst = NULL;
	while (envp[len] != NULL)
		len++;
	while (i != len)
	{
		new = ft_lstnew(ft_strdup(envp[i]));
		if (!new)
		{
			ft_lstclear(&env->lst, free);
			free(envp);
		}
		ft_lstadd_back(&env->lst, new);
		i++;
	}
	// free_env(env);
	// print_env(env);
	get_env_names_and_values(env);
}

void	get_env_names_and_values(t_env *env)
{
	t_list	*cur;
	char	*limit;
	int		len;

	len = 0;
	cur = env->lst;
	while (cur != NULL)
	{
		limit = ft_strchr(cur->content, '=');
		if (limit)
		{
			len = limit - (char *)cur->content;
			env->name = malloc(sizeof(char) * len + 1);
			// if (!env->name)
			// 	return;
			ft_strncpy(env->name, cur->content, len);
			env->value = ft_strdup(limit + 1);
			// if (!env->value)
			// 	return;
		}
		else
		{
			env->name = strdup(cur->content);
			env->value = NULL;
		}
		// printf("name = %s\n", env->name);
		// printf("value = %s\n", env->value);
		cur = cur->next;
	}
	while(cur != NULL)
		ft_lstclear(&cur, free);
	free_env(env);
}

int main(int argc, char **argv, const char **envp)
{
	argc = 0;
	if (argv[0])
		init_env(envp);
	return (0);
}
