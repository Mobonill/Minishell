/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 11:47:34 by zserobia          #+#    #+#             */
/*   Updated: 2024/12/02 11:47:37 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	update_env(t_env *env, const char *var_name, const char *new_value)
{
	while (env)
	{
		if (ft_strcmp(env->name, var_name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(new_value);
			printf("NEW %s now %s\n", env->name, env->value);
			return ;
		}
		env = env->next;
	}
	printf("bash: Environment variable %s not found. ", var_name);
	printf("Please implement insertion.\n");
}

void	cd_to_path(const char *path, t_env *env)
{
	char	*current_path;
	char	*new_path;

	current_path = getcwd(NULL, 0);
	printf("current path = %s\n", current_path);
	if (!current_path)
	{
		perror("bash: cd: error getting current directory");
		return ;
	}
	if (chdir(path) == -1)
		printf("cd: %s: no such file or directory\n", path);
	else
	{
		new_path = getcwd(NULL, 0);
		printf("new_path = %s\n", new_path);
		if (new_path)
		{
			update_env(env, "OLDPWD", current_path);
			update_env(env, "PWD", new_path);
			free(new_path);
		}
	}
	free(current_path);
}

void	cd_to_home(t_env *env)
{
	char	*home_path;

	home_path = get_env_value("HOME", env);
	if (!home_path)
	{
		printf("bash: cd: HOME not set\n");
		return ;
	}
	cd_to_path(home_path, env);
}

void	cd_to_previous(t_env *env)
{
	char	*old_path;

	old_path = get_env_value("OLDPWD", env);
	printf("OLD PATH %s\n", old_path);
	if (!old_path)
	{
		printf("bash: cd: OLDPWD not set\n");
		return ;
	}
	cd_to_path(old_path, env);
}
/*
void cd_to_previous(t_env *env)
{
    char *old_path;

    old_path = get_env_value("OLDPWD", env);
    printf("OLD PATH %s\n", old_path);
    if (!old_path) {
        printf("bash: cd: OLDPWD not set\n");
        return;
    }

    // Save the current PWD as OLDPWD before changing directories
    char *current_path = getcwd(NULL, 0);
    if (!current_path) {
        perror("bash: cd: error getting current directory");
        return;
    }

    // Change to the OLDPWD directory
    cd_to_path(old_path, env);  // Change directory to OLDPWD

    // After changing directory, we need to update OLDPWD to the previous PWD
    update_env(env, "OLDPWD", current_path);
    free(current_path);
}*/

int	builtin_cd(t_simple_cmds *cmd, t_env *env)
{
	int	arg_count;

	arg_count = 0;
	if (!cmd || !cmd->str || !cmd->str[0])
		return (1);
	while (cmd->str[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		printf("bash: cd: too many arguments\n");
		g_global_exit = 1;
	}
	else if (arg_count == 1
		|| (arg_count == 2 && ft_strcmp(cmd->str[1], "--") == 0))
		cd_to_home(env);
	else if (arg_count == 2 && ft_strcmp(cmd->str[1], "-") == 0)
		cd_to_previous(env);
	else
		cd_to_path(cmd->str[1], env);
	return (0);
}
