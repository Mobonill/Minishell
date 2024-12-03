/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_create2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:59:08 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/25 14:59:11 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
