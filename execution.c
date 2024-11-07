/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:43:01 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/07 18:27:02 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

int	my_choosen_exec(char *str, t_shell *shell)
{
	char *unset[] = {
		"UNSET" ,
		"PAGER" ,
		"LESS",
		NULL
	};
	char *export[] = {
		"EXPORT" ,
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
		return (1);
		// execute_minishell();
}
