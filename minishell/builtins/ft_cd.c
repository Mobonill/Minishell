/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:22:27 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:34:53 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	update_env(t_env *env, const char *var_name, const char *new_value)
{
	while (env)
	{
		if (ft_strcmp(env->name, var_name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(new_value);
			return ;
		}
		env = env->next;
	}
}

int	change_directory(const char *path, char **current_path,
	char **new_path, t_env *env)
{
	*current_path = getcwd(NULL, 0);
	if (!*current_path)
		*current_path = ft_strdup(get_env_value("PWD", env));
	if (!*current_path)
	{
		ft_fprintf(2, "bash: cd: unable to determine current directory\n");
		g_global_exit = 1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "bash: cd: %s: No such file or directory\n", path);
		free(*current_path);
		g_global_exit = 1;
		return (1);
	}
	*new_path = getcwd(NULL, 0);
	if (!*new_path)
	{
		ft_fprintf(2, "bash: cd: unable to determine new directory\n");
		free(*current_path);
		return (1);
	}
	return (0);
}

int	cd_to_path(const char *path, t_env *env)
{
	char	*current_path;
	char	*new_path;

	current_path = NULL;
	new_path = NULL;
	if (change_directory(path, &current_path, &new_path, env) != 0)
		return (1);
	update_env(env, "OLDPWD", current_path);
	update_env(env, "PWD", new_path);
	free(current_path);
	free(new_path);
	g_global_exit = 0;
	return (0);
}

int	cd_to_home(t_env *env)
{
	char	*home_path;

	home_path = get_env_value("HOME", env);
	if (!home_path)
	{
		ft_fprintf(2, "bash: cd: HOME not set\n");
		g_global_exit = 1;
		return (1);
	}
	return (cd_to_path(home_path, env));
}

int	builtin_cd(t_simple_cmds *cmd, t_env *env)
{
	int	arg_count;
	int	result;

	result = 0;
	arg_count = 0;
	if (!cmd || !cmd->str || !cmd->str[0])
	{
		g_global_exit = 1;
		return (1);
	}
	while (cmd->str[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_fprintf(2, "bash: cd: too many arguments\n");
		g_global_exit = 1;
		return (1);
	}
	else if (arg_count == 1)
		result = cd_to_home(env);
	else
		result = cd_to_path(cmd->str[1], env);
	return (result);
}
