/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:22:27 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/18 15:55:38 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void update_env(t_env *env, const char *var_name, const char *new_value)
{
	while (env)
	{
		if (ft_strcmp(env->name, var_name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(new_value);
			printf("NEW %s now %s\n", env->name, env->value); // del
			return;
		}
		env = env->next;
	}
	// printf("bash: Environment variable %s not found. ", var_name);
	// printf("Please implement insertion if needed.\n");
}

void cd_to_path(const char *path, t_env *env)
{
	char *current_path = getcwd(NULL, 0);
	char *new_path = NULL;

	if (!current_path)
		current_path = ft_strdup(get_env_value("PWD", env));
	if (!current_path)
	{
		printf("bash: cd: unable to determine current directory\n");
		return;
	}

	if (chdir(path) == -1)
	{
		printf("bash: cd: %s: No such file or directory\n", path);
		free(current_path);
		return;
	}

	new_path = getcwd(NULL, 0);
	if (!new_path)
	{
		printf("bash: cd: unable to determine new directory\n");
		free(current_path);
		return;
	}
	update_env(env, "OLDPWD", current_path);
	update_env(env, "PWD", new_path);
	free(current_path);
	free(new_path);
}

void cd_to_home(t_env *env)
{
	char *home_path;

	home_path = get_env_value("HOME", env);
	if (!home_path)
	{
		printf("bash: cd: HOME not set\n");
		return;
	}
	cd_to_path(home_path, env);
}

int builtin_cd(t_simple_cmds *cmd, t_env *env)
{
	int arg_count;

	arg_count = 0;
	if (!cmd || !cmd->str || !cmd->str[0])
		return (1);
	while (cmd->str[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		printf("bash: cd: too many arguments\n");
		g_global_exit = 1;
		return (1);
	}
	else if (arg_count == 1)
		cd_to_home(env);
	else
		cd_to_path(cmd->str[1], env);
	return (0);
}
