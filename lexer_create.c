/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:06:27 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:06:30 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_lexer	*lexer_create(char *value, t_tokens token, int index)
{
	t_lexer	*new;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
		return (NULL);
	new->str = value;
	new->token = token;
	new->index = index;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	lexer_add_back(t_lexer **list, t_lexer *new_token)
{
	t_lexer	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
}

void	ft_create_lexer_list(char *value, t_tokens type, int index,
	t_lexer **list)
{
	t_lexer	*new_token;

	new_token = lexer_create(value, type, index);
	if (new_token)
		lexer_add_back(list, new_token);
}

int	ft_read_word(char *line)
{
	int	i;

	i = 0;
	while (line[i] && !ft_ifspace(line[i]) && line[i] != '|' && line[i] != '>'
		&& line[i] != '<' && line[i] != '\'' && line[i] != '"')
		i++;
	return (i);
}

int	ft_read_word_quote(char *line, char quote)
{
	int	i;

	i = 1;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		i++;
	return (i);
}

int	ft_process1(char **line)
{
	char	quote;
	int		len;

	len = 0;
	if (**line == '\'' || **line == '"')
	{
		quote = **line;
		len = ft_read_word_quote(*line, quote);
	}
	else
		len = ft_read_word(*line);
	return (len);
}

char	*ft_process2(char *result, char *temp)
{
	char	*old_result;

	old_result = result;
	result = ft_strjoin(result, temp);
	free(temp);
	free(old_result);
	return (result);
}
