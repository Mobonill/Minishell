/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:55:59 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/27 18:56:02 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <stddef.h>

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}

t_env	*create_new_env_node1(char *content, int index)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
	{
		printf("malloc failed for new environment node");
		return (NULL);
	}
	new_env->content = ft_strdup(content);
	if (!new_env->content)
	{
		printf("strdup failed for environment variable content");
		free(new_env);
		return (NULL);
	}
	new_env->index = index;
	new_env->next = NULL;
	return (new_env);
}

int	split_name_value(t_env *new_env)
{
	char	*equals_pos;
	int		name_len;

	equals_pos = ft_strchr(new_env->content, '=');
	if (equals_pos)
	{
		name_len = equals_pos - new_env->content;
		new_env->name = malloc(name_len + 1);
		if (!new_env->name)
		{
			printf("malloc failed for environment variable name");
			return (-1);
		}
		ft_strncpy(new_env->name, new_env->content, name_len);
		new_env->name[name_len] = '\0';
		new_env->value = ft_strdup(equals_pos + 1);
		if (!new_env->value)
		{
			free(new_env->name);
			return (-1);
		}
	}
	else
	{
		new_env->name = NULL;
		new_env->value = NULL;
	}
	return (0);
}

void	add_env_node(t_shell *shell, t_env *new_env)
{
	t_env	*last_env;

	last_env = shell->env;
	if (!last_env)
	{
		shell->env = new_env;
	}
	else
	{
		while (last_env->next)
			last_env = last_env->next;
		last_env->next = new_env;
	}
}

void	init_env(char **envp, t_shell *shell)
{
	int		i;
	t_env	*new_env;

	i = 0;
	shell->env = NULL;
	while (envp[i])
	{
		new_env = create_new_env_node1(envp[i], i);
		if (!new_env)
			return ;
		if (split_name_value(new_env) != 0)
		{
			free(new_env->content);
			free(new_env);
			return ;
		}
		add_env_node(shell, new_env);
		i++;
	}
}
