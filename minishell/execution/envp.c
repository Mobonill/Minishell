/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:45:44 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/20 14:49:42 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

t_env	*ft_envnew(char *content)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->content = content;
	env->next = NULL;
	return (env);
}

t_env	*ft_envlast(t_env *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}

void	ft_env_add_back(t_env **env, t_env *new)
{
	t_env	*nlast;

	if (!(*env))
	{
		*env = new;
		return ;
	}
	nlast = ft_envlast(*env);
	nlast->next = new;
}

t_env	*init_env(const char **envp, t_shell *shell)
{
	t_env	*new_env;
	int		i;

	i = 0;
	shell->env = NULL;
	new_env = NULL;
	while (envp[i])
	{
		new_env = ft_envnew(ft_strdup(envp[i]));
		new_env->index = i;
		if (!new_env)
		{
			ft_envclear(&new_env, free);
			free(envp);
		}
		if (shell->env == NULL)
			shell->env = new_env;
		else
			ft_env_add_back(&shell->env, new_env);
		i++;
	}
	get_env_names_and_values(shell->env);
	return (shell->env);
}

void	get_env_names_and_values(t_env *env)
{
	t_env	*cur;
	char	*limit;
	int		len;

	len = 0;
	cur = env;
	while (cur != NULL)
	{
		limit = ft_strchr(cur->content, '=');
		if (limit)
		{
			{
				len = limit - cur->content;
				cur->name = malloc(sizeof(char) * (len + 1));
				if (!cur->name)
					return ;
				ft_strncpy(cur->name, cur->content, len);
				cur->name[len] = '\0';
				cur->value = ft_strdup(limit + 1);
				if (!cur->value)
					return ;
			}
		}
		cur = cur->next;
	}
}
