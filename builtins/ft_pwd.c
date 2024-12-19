/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:33:04 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/23 16:24:43 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_current_path(t_env *env)
{
	char	*current_path;

	current_path = getcwd(NULL, 0);
	if (!current_path)
		current_path = ft_strdup(get_env_value("PWD", env));
	return (current_path);
}

void	ft_pwd(t_env *env)
{
	char	*path;

	path = get_current_path(env);
	if (!path)
	{
		printf("bash: pwd: No such file or directory\n");
		g_global_exit = 1;
		return;
	}
	printf("%s\n", path);
	free(path);
	g_global_exit = 0;
}