/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:19:48 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/20 15:09:18 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_envsize_minishell(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	create_exec_env(t_shell *shell, t_exec *exec, char *cmd_path,
	t_simple_cmds *parser)
{
	exec->env = transform_env_list_to_tab(shell, exec, 0);
	cmd_path = find_path(parser, shell);
	ft_cmd_path(cmd_path, parser, exec);
}

char	**transform_env_list_to_tab(t_shell *shell, t_exec *exec, int i)
{
	t_env	*cur;
	int		size_env;

	cur = shell->env;
	size_env = ft_envsize_minishell(shell->env);
	exec->env = malloc(sizeof(char *) * (size_env + 1));
	if (!exec->env)
		return (NULL);
	exec->env[size_env] = NULL;
	while (cur)
	{
		exec->env[i] = ft_strdup(cur->content);
		if (!exec->env[i])
		{
			while (--i >= 0)
				free(exec->env[i]);
			free(exec->env);
			exec->env = NULL;
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	return (exec->env);
}

int	cleanup_and_exit(t_exec *exec, t_shell *shell)
{
	(void)shell;
	if (exec->env)
		free_exec_env(exec->env);
	return (errno);
}
