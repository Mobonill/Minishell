/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:43:21 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:03:31 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	save_and_restore_std(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
}

void	restore_std(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_builtin(t_simple_cmds *parser, t_shell *shell, t_exec *exec)
{
	int	saved_stdin;
	int	saved_stdout;

	save_and_restore_std(&saved_stdin, &saved_stdout);
	if (handle_redirections_and_heredoc(exec, parser, 0) < 0)
	{
		restore_std(saved_stdin, saved_stdout);
		return (-1);
	}
	if (!ft_strcmp(parser->str[0], "cd"))
		g_global_exit = builtin_cd(parser, shell->env);
	else if (!ft_strcmp(parser->str[0], "export"))
		g_global_exit = builtin_export(parser, shell);
	else if (!ft_strcmp(parser->str[0], "unset"))
		ft_unset(parser->str, shell);
	else if (!ft_strcmp(parser->str[0], "env"))
		ft_env(shell->env);
	else if (!ft_strcmp(parser->str[0], "exit"))
		g_global_exit = builtin_exit(parser, shell);
	else if (!ft_strcmp(parser->str[0], "echo"))
		g_global_exit = builtin_echo(parser);
	else if (!ft_strcmp(parser->str[0], "pwd"))
		ft_pwd(shell->env);
	restore_std(saved_stdin, saved_stdout);
	return (g_global_exit);
}
