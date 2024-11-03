/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:32:37 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/03 15:34:38 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void initialization_exec_struct(t_exec exec)
{
	exec.exit_status = 1; // si la cmd s'execute avec succes, exit.status = 0;
}

void	check_my_parseur(t_list parseur)
{
	
}

int	choose_exec(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return 0;
		// return (ft_echo); // REPRODUIT LE FONCTIONNEMENT DE LA CMD ECHO, NE PAS CREER DE CHILD
	else if (!ft_strcmp(cmd, "cd"))
			return 0;
		// return (ft_cd); ( ? =cd retourne 1 en cas d erreur dee chemin)
	else if (!ft_strcmp(cmd, "pwd"))
		return 0;
		// return (ft_pwd);
	else if (!ft_strcmp(cmd, "export"))
		return 0;
		// return (ft_export);
	else if (!ft_strcmp(cmd, "unset"))
		return 0;
		// return (unset);
	else if(!ft_strcmp(cmd, "env"))
		return 0;
		// return (ft_env);
	else if (!ft_strcmp(cmd, "exit"))
		return 0;
		// return (ft_exit);
	else
		execute_minishell();
}

void execute_minishell()
{
	if ()
	 int execve(const char *pathname, char *const argv[], char *const envp[])
}

void	execute_command(char *cmd, t_pipes *pipes)
{
	char	*cmd_path;
	char	**cmd_argv;

	cmd_argv = ft_split(cmd, ' '); // en fonction des tokens
	cmd_path = find_path(cmd_argv[0], pipes->envp);
	if (!cmd_path)
	{
		ft_fprintf(2, "%s: command not found\n", cmd_argv[0]);
		free_cmd_argv(cmd_argv);
		free_all(pipes);
		exit(127);
	}
	if (execve(cmd_path, cmd_argv, pipes->envp) == -1)
	{
		perror("Execve error");
		free_cmd_argv(cmd_argv);
		free(cmd_path);
		free_all(pipes);
		exit(126);
	}
	free(cmd_path);
	free_cmd_argv(cmd_argv);
}