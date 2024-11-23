/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_trouve_len.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:30:20 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 15:30:22 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	handle_env_variable(const char *input, int *i, int *len, char **envp)
{
	int		start;
	char	*key;
	char	*value;

	start = *i;
	while (isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	key = ft_substr(input, start, *i - start);
	value = get_env_value(key, envp);
	if (value)
		*len += strlen(value);
	free(key);
}

void	handle_variable(const char *input, int *i, int *len, char **envp)
{
	(*i)++;
	if (input[*i] == '\0' || ft_ifspace(input[*i]) || input[*i] == '\'')
		(*len)++;
	else if (input[*i] == '?')
	{
		*len += 11;
		(*i)++;
	}
	else if (isdigit(input[*i]))
	{
		(*i)++;
		while (isdigit(input[*i]))
		{
			(*i)++;
			(*len)++;
		}
	}
	else if (isalpha(input[*i]) || input[*i] == '_')
		handle_env_variable(input, i, len, envp);
	else
		(*i)++;
}

void	handle_quote(const char *input, int *i, char *quote, int *len)
{
	if (*quote == '\0')
		*quote = input[*i];
	else if (*quote == input[*i])
		*quote = '\0';
	else
		(*len)++;
	(*i)++;
}

int	ft_trouve_len(const char *input, char **envp)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = 0;
	quote = '\0';
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			handle_quote(input, &i, &quote, &len);
		else if (input[i] == '$' && quote != '\'')
			handle_variable(input, &i, &len, envp);
		else
		{
			len++;
			i++;
		}
	}
	return (len + 1);
}
