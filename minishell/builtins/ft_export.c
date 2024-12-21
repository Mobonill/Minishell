/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:54:56 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:34:32 by mobonill         ###   ########.fr       */
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
		ft_fprintf(2, "export: `%s': not a valid identifier\n", arg);
		g_global_exit = 1;
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
	g_global_exit = 0;
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
