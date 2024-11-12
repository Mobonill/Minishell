/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/12 16:29:34 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

int	my_choosen_exec(char *str, t_shell *shell)
{
	char *unset[] = {
		"UNSET" ,
		"1pager" ,
		"_less",
		"PWD",
		NULL
	};

	char *export[] = {
		"EXPORT" ,
		"LESS=2TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT1",
		"ttttttttttttttttt=tttttttttttt" ,
		"452ffffffffffffffffffffff",
		"_ttttttttttttttttttt"
		"-ffffffffffffffffffffffff"
		"tttttttttt754t",
		"t=454ttttttttttttttttttttt",
		NULL
	};
	// t_env *cur = shell->env;
	// while(cur != NULL)
	// {
	// 	printf("%s", cur->name);
	// 	printf("=");
	// 	printf("%s\n", cur->value);
	// 	cur = cur->next;
	// }
	
	// si on a un shell->hidden, on strcmp(cdm, hidden) et si c'est == et qu'il y a un = juste apres, on entre une value et on add le node.
	// // if (shell->hidden)
	// // {
	// 		cur = 
	// // 		while ()
	// // }
	if (!ft_strcmp(str, "ECHO"))
		return 0;
		// return (ft_cd); ( ? =cd retourne 1 en cas d erreur dee chemin)
	else if (!ft_strcmp(str, "PWD"))
		return (ft_pwd(shell->env), 0);
	else if (!ft_strcmp(export[0], "EXPORT"))
		return (ft_export(export, shell), 0);
	else if (!ft_strcmp(unset[0], "UNSET"))
		return (ft_unset(unset, shell), 0);
	else if(!ft_strcmp(str, "ENV"))
		return (ft_env(shell->env), 0);
	else if (!ft_strcmp(str, "EXIT"))
		return 0;
		// return (ft_exit);
	else
		execute_minishell();
}

void	execute_minishell(t_shell *shell, t_parser *parser)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	exec->env = 
	exec->path = find_path(parser->cmd[0], shell);
	exec->num_pipes = 
	exec->fd

	exec->pid = malloc(sizeof(pid_t) * exec->num_cmds);
	int	i;

	i = -1;
	exec->fd = malloc(sizeof(int *) * exec->num_pipes);
	if (!exec->fd)
		return (errno);
	while (++i < pipes->num_pipes)
	{
		pipes->fd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes->fd[i]) == -1)
		{
			free_pipes(pipes);
			return (errno);
		}
	}
}
