/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_read_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:54:02 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 14:54:05 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*process_quoted_or_unquoted(char **line, int *total_len)
{
	char	*temp;
	char	*result;
	int		len;

	temp = NULL;
	result = NULL;
	while (**line && !ft_ifspace(**line) && **line != '|'
		&& **line != '<' && **line != '>')
	{
		len = ft_process1(line);
		temp = ft_substr(*line, 0, len);
		if (result)
			result = ft_process2(result, temp);
		else
			result = temp;
		*total_len += len;
		*line += len;
		if (!result && temp)
			free(temp);
	}
	return (result);
}

int	ft_read_token_1(char *line, t_lexer **list, int index)
{
	int		len;
	int		total_len;
	char	*word;
	char	*temp;

	total_len = 0;
	word = NULL;
	while (*line && !ft_ifspace(*line) && *line != '|'
		&& *line != '<' && *line != '>')
	{
		temp = process_quoted_or_unquoted(&line, &total_len);
		word = ft_strjoin_free(word, temp);
		free(temp);
	}
	if (word)
		ft_create_lexer_list(word, WORD, index, list);
	return (total_len);
}

int	create_token(char *value, int type, t_lexer **list, int *index)
{
	ft_create_lexer_list(strdup(value), type, (*index)++, list);
	return (strlen(value));
}

int	special_tokens(char *line, t_lexer **list, int *index, t_shell *shell)
{
	if (*line == '|')
	{
		shell->count_pipe++;
		return (create_token("|", PIPE, list, index));
	}
	else if (*line == '>' && *(line + 1) == '>')
		return (create_token(">>", APPEND, list, index));
	else if (*line == '>')
		return (create_token(">", OUT, list, index));
	else if (*line == '<' && *(line + 1) == '<')
		return (create_token("<<", HEREDOC, list, index));
	else if (*line == '<')
		return (create_token("<", IN, list, index));
	return (0);
}

void	ft_read_token(char *line, t_lexer **list, t_shell *shell)
{
	int	index;
	int	len;

	index = 0;
	while (*line)
	{
		while (ft_ifspace(*line))
			line++;
		len = 0;
		if (!*line)
			break ;
		len = special_tokens(line, list, &index, shell);
		if (len == 0)
			len = ft_read_token_1(line, list, index++);
		line += len;
	}
}
