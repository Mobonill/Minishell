/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:32:37 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/05 18:49:25 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

// void initialization_exec_struct(t_exec exec)
// {
// 	exec.exit_status = 1; // si la cmd s'execute avec succes, exit.status = 0;
// }

// void	parent_process(t_parser *parser)
// {
// 	choose_exec(&parser);
// }

// l 'expandeur devra renvoyer les commandes et les separer correctement par des pipes si cela n'a pas ete fait

int	choose_exec(char *str, t_env *env) //str = parser->cmd
{
	char *unset[] = {
		"UNSET" ,
		"PAGER" ,
		"LESS",
		NULL
	};

	if (!ft_strcmp(str, "ECHO"))
		return 0;
		// return (ft_echo); // REPRODUIT LE FONCTIONNEMENT DE LA CMD ECHO, NE PAS CREER DE CHILD
	else if (!ft_strcmp(str, "CD"))
		return 0;
		// return (ft_cd); ( ? =cd retourne 1 en cas d erreur dee chemin)
	else if (!ft_strcmp(str, "PWD"))
		return (ft_pwd(env), 0);
	else if (!ft_strcmp(str, "EXPORT"))
		return 0;
		// return (ft_export);
	else if (!ft_strcmp(unset[0], "UNSET"))
		return (ft_unset(unset, env), 0);
	else if(!ft_strcmp(str, "ENV"))
		return (ft_env(env), 0);
	else if (!ft_strcmp(str, "EXIT"))
		return 0;
		// return (ft_exit);
	else
		return (1);
		// execute_minishell();
}

// void execute_minishell()
// {
// 	if ()
// 	 int execve(const char *pathname, char *const argv[], char *const envp[])
// }

// void	execute_command(char *cmd, t_pipes *pipes)
// {
// 	char	*cmd_path;
// 	char	**cmd_argv;

// 	cmd_argv = ft_split(cmd, ' '); // en fonction des tokens
// 	cmd_path = find_path(cmd_argv[0], pipes->envp);
// 	if (!cmd_path)
// 	{
// 		ft_fprintf(2, "%s: command not found\n", cmd_argv[0]);
// 		free_cmd_argv(cmd_argv);
// 		free_all(pipes);
// 		exit(127);
// 	}
// 	if (execve(cmd_path, cmd_argv, pipes->envp) == -1)
// 	{
// 		perror("Execve error");
// 		free_cmd_argv(cmd_argv);
// 		free(cmd_path);
// 		free_all(pipes);
// 		exit(126);
// 	}
// 	free(cmd_path);
// 	free_cmd_argv(cmd_argv);
// }