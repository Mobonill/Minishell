/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:18:35 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 15:18:38 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	**create_command(t_lexer *lexer)
{
	char	**cmd;
	int		i;
	int		len;

	if (!lexer)
		return (NULL);
	len = count_cmd(lexer);
	cmd = malloc(sizeof(char *) * (len + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	while (i < len)
	{
		cmd[i] = ft_strdup(lexer->str);
		if (!cmd[i])
		{
			free_command(cmd);
			return (NULL);
		}
		lexer = lexer->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

t_simple_cmds	*new_simple_cmd(t_lexer *lexer, t_pars_mini *pars_mini)
{
	t_simple_cmds	*new;

	new = malloc(sizeof(t_simple_cmds));
	if (!new)
		return (NULL);
	new->redirections = pars_mini->redirections;
	new->num_redirections = pars_mini->num_redirections;
	new->str = create_command(lexer);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_simple_cmds	*last_simple_cmd(t_simple_cmds *list)
{
	if (list)
	{
		while (list->next != NULL)
			list = list->next;
	}
	return (list);
}

void	add_new_simple_cmd(t_simple_cmds **list, t_simple_cmds *new)
{
	t_simple_cmds	*last;

	if (*list)
	{
		last = last_simple_cmd(*list);
		last->next = new;
		new->prev = last;
	}
	else
		*list = new;
}

t_simple_cmds	*create_and_add_command(t_lexer *tmp,
	t_pars_mini *pars_mini, t_simple_cmds **commands)
{
	t_simple_cmds	*new_cmd;

	new_cmd = new_simple_cmd(tmp, pars_mini);
	if (!new_cmd)
	{
		printf("Error creating new command node\n");
		return (NULL);
	}
	add_new_simple_cmd(commands, new_cmd);
	return (new_cmd);
}
