/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:01:04 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:01:07 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_tokens(t_lexer *list)
{
	t_lexer	*tmp;

	tmp = list;
	while (tmp)
	{
		printf("Index: %d, Type: %d, Value: %s\n", tmp->index, tmp->token, tmp->str);
		tmp = tmp->next;
	}
}

void	print_simple_cmds(t_simple_cmds *cmds)
{
	while (cmds)
	{
		printf("Command: ");
		for (int i = 0; cmds->str && cmds->str[i]; i++)
		{
			printf("%s ", cmds->str[i]);
		}
		printf("\n");
		printf("Number of redirections: %d\n", cmds->num_redirections);
		t_lexer *redir = cmds->redirections;
		while (redir)
		{
			printf("Redirection: Token: %d, Str: %s\n",
				redir->token, redir->str);
			redir = redir->next;
		}
		printf("\n");
		cmds = cmds->next;
	}
}

int	ft_ifspace(char line)
{
	return (line == ' ' || (line >= 9 && line <= 13));
}
