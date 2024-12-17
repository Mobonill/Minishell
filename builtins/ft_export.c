/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:54:56 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/17 19:49:23 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (false);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

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
	printf("Updated value: %s\n", cur->value);
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

void	create_new_env_node(char *to_compare, t_env **env,
		char *limit, char *export)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	initialize_env_node(new_node, export);
	if (!new_node->content)
		return ;
	new_node->name = ft_strdup(to_compare);
	if (!new_node->name)
	{
		free(new_node->content);
		free(new_node);
		return ;
	}
	if (limit && *(limit + 1))
		new_node->value = ft_strdup(limit + 1);
	else if (limit)
		new_node->value = ft_strdup("");
	else
		new_node->value = NULL;
	ft_env_add_back(env, new_node);
}

void	print_sorted_env(t_env *env)
{
	while (env)
	{
		printf("%s", env->name);
		if (env->value)
			printf("=%s", env->value);
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
			if (current && current->next
				&& ft_strcmp(current->name, current->next->name) > 0)
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

void	handle_new_or_existing_env(char *to_compare, char *limit,
		char *export, t_env **env)
{
	t_env	*cur;
	bool	exist;

	cur = *env;
	exist = false;
	while (cur)
	{
		if (ft_strcmp(to_compare, cur->name) == 0)
		{
			modify_env_value(cur, limit);
			exist = true;
			break ;
		}
		cur = cur->next;
	}
	if (!exist)
		create_new_env_node(to_compare, env, limit, export);
}

void	process_export_argument(char *arg, t_env **env)
{
	char	*limit;
	char	*to_compare;

	if (!is_valid_identifier(arg))
	{
		printf("export: `%s': not a valid identifier\n", arg);
		return ;
	}
	limit = ft_strchr(arg, '=');
	if (limit)
	{
		to_compare = ft_strndup(arg, limit - arg);
		handle_new_or_existing_env(to_compare, limit, arg, env);
		free(to_compare);
	}
	else
	{
		to_compare = ft_strdup(arg);
		handle_new_or_existing_env(to_compare, NULL, arg, env);
		free(to_compare);
	}
}

int	builtin_export(t_simple_cmds *simple_cmd, t_shell *shell)
{
	int	i;

	i = 1;
	while (simple_cmd->str[i])
	{
		process_export_argument(simple_cmd->str[i], &shell->env);
		i++;
	}
	sort_env_list(shell->env);
	if (!simple_cmd->str[1])
		print_sorted_env(shell->env);
	return (0);
}
