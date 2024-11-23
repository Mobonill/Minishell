/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:05:00 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 18:05:02 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_pars(t_simple_cmds *pars)
{
	t_simple_cmds	*current;
	t_simple_cmds	*next;
	int				i;

	i = 0;
	current = pars;
	while (current != NULL)
	{
		next = current->next;
		if (current->str)
		{
			while (current->str[i] != NULL)
			{
				free(current->str[i]);
				i++;
			}
			free(current->str);
		}
		if (current->hd_file_name)
			free(current->hd_file_name);
		if (current->redirections)
			free_lexer_list(&current->redirections);
		free(current);
		current = next;
	}
}

void	ft_free_pars(t_shell *shell)
{
	if (shell->pars)
		free_pars(shell->pars);
	if (shell->pars_mini)
	{
		free(shell->pars_mini);
		shell->pars_mini = NULL;
	}
}

void	ft_free_tous(t_shell *shell)
{
	free_simple_cmds_list(shell->commands);
	free_lexer_list(&shell->lexer_list);
	free(shell->pars_mini);
	shell->pars_mini = NULL;
	if (shell->input_line)
	{
		free(shell->input_line);
		shell->input_line = NULL;
	}
}

void	free_command(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}
