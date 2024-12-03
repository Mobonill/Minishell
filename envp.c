/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:45:44 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/03 16:22:41 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	ft_envclear(t_env **env, void (*del)(void *))
{
	t_env	*temp;

	while (*env)
	{
		temp = (*env)->next;
		del((*env)->content);
		if ((*env)->name)
			free((*env)->name);
		if ((*env)->value)
			free((*env)->value);
		free(*env);
		*env = temp;
	}
}
void	free_env(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur != NULL)
	{
		ft_envclear(&cur, free);
	}
}

void	print_env(t_env *env)
{
	t_env *cur = env;
	
	while (cur != NULL)
	{
		printf("name = %s\n", cur->name);
		printf("value = %s\n", cur->value);
		printf("index = %d\n", cur->index);
		cur = cur->next;
	}
}

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
			len = limit - (char *)cur->content;
			cur->name = malloc(sizeof(char) * len + 1);
			ft_strncpy(cur->name, cur->content, len);
			cur->value = ft_strdup(limit + 1);
		}
		cur = cur->next;
	}
	// print_env(env);
	// free_env(env);
}

// int main(int argc, char **av, const char **envp)
// {
// 	t_shell *shell = NULL;
	
// 	argc = 0;
// 	if (av[0])
// 		shell = init_env(envp, shell);
// 	free(shell);
// 	return (0);
// }
