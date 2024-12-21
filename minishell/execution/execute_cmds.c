/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:09:10 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:10:23 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	ft_cmd_path(char *cmd_path, t_simple_cmds *parser, t_exec *exec)
{
	if (cmd_path)
	{
		if (execve(cmd_path, parser->str, exec->env) == -1)
		{
			perror(parser->str[0]);
			free(cmd_path);
			free_cmd_argv(parser);
			free_all(exec);
			g_global_exit = 126;
			exit(126);
		}
	}
	else
	{
		ft_fprintf(STDERR_FILENO, "%s: command not found\n", parser->str[0]);
		free_cmd_argv(parser);
		free_all(exec);
		g_global_exit = 127;
		exit(127);
	}
}

void	execute_command(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	char	*cmd_path;

	cmd_path = NULL;
	while (parser != NULL)
	{
		if (!parser || !parser->str || !parser->str[0])
		{
			parser = parser->next;
			continue ;
		}
		create_exec_env(shell, exec, cmd_path, parser);
		free(cmd_path);
		free_cmd_argv(parser);
		free_all(exec);
		if (cmd_path)
			free(cmd_path);
	}
}

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo"));
}
