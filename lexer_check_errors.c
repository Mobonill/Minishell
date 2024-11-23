/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:01:34 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:01:37 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	ft_minicheck(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	while (ft_ifspace(line[i]))
		i++;
	if (i == len)
		return (1);
	if (line[i] == '|')
	{
		printf("-bash: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	find_matching_quote(char *line, int start, char quote)
{
	int	i;

	i = start + 1;
	while (line[i])
	{
		if (line[i] == quote)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

int	check_quotes(char *line)
{
	int	i;
	int	closing_index;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
		{
			closing_index = find_matching_quote(line, i, '"');
			if (closing_index == -1)
				return (0);
			i = closing_index;
		}
		else if (line[i] == '\'')
		{
			closing_index = find_matching_quote(line, i, '\'');
			if (closing_index == -1)
				return (0);
			i = closing_index;
		}
		i++;
	}
	return (1);
}

int	ft_check_errors(t_lexer *list)
{
	t_lexer	*tmp;

	tmp = list;
	while (tmp)
	{
		if ((tmp->token == PIPE && tmp->next && tmp->next->token == PIPE)
			|| (tmp->token == PIPE && !tmp->next))
		{
			printf("-bash: syntax error near unexpected token `|'\n");
			return (1);
		}
		else if (tmp->token > PIPE && !tmp->next)
		{
			printf("-bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
		else if (tmp->token > PIPE && tmp->next && tmp->next->token >= PIPE)
		{
			printf("-bash: syntax error near unexpected token `%s'\n",
				tmp->next->str);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
