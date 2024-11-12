/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 17:22:36 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/12 17:55:38 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

char	*get_envp_path(t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur != NULL)
	{
		if (ft_strncmp(cur->name, "PATH", 4) == 0)
			return (ft_substr(cur->value));
		cur = cur->next;
	}
	return (NULL);
}

char	*find_command_in_path(char *cmd, char **path)
{
	char	*cmd_path;
	char	*temp_path;
	int		i;

	i = 0;
	while (path[i])
	{
		temp_path = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

void	free_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
}

char	*find_path(char *cmd, t_shell *shell)
{
	char	**path;
	char	*envp_path;
	char	*cmd_path;

	envp_path = get_envp_path(shell->env);
	if (!envp_path)
	{
		perror("PATH NOT FOUND IN ENVP");
		return (NULL);
	}
	path = ft_split(envp_path, ':');
	free(envp_path);
	cmd_path = find_command_in_path(cmd, path);
	free_path(path);
	return (cmd_path);
}
