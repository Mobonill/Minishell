/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:37:38 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 15:37:41 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	han_quote1(const char *input, char *quote, t_temp *temp, char *result)
{
	if (*quote == '\0')
		*quote = input[temp->i++];
	else if (*quote == input[temp->i])
	{
		*quote = '\0';
		temp->i++;
	}
	else
		result[temp->j++] = input[temp->i++];
}

char	*allocate_result(int len)
{
	char	*result;

	result = malloc(len + 1);
	if (!result)
	{
		printf("malloc failed");
		exit(EXIT_FAILURE);
	}
	return (result);
}

char	*process_str(const char *input, t_shell *shell)
{
	int		len;
	char	*result;
	char	quote;
	t_temp	temp;

	len = ft_trouve_len(input, shell->envp);
	result = allocate_result(len);
	temp = (t_temp){0, 0};
	quote = '\0';
	while (input[temp.i])
	{
		if (input[temp.i] == '\'' || input[temp.i] == '\"')
			han_quote1(input, &quote, &temp, result);
		else if (input[temp.i] == '$' && quote != '\''
			&& input[temp.i + 1] != '\0'
			&& !ft_ifspace(input[temp.i + 1]) && input[temp.i + 1] != '\''
			&& input[temp.i + 1] != '"')
			handle_dollar(shell, input, &temp, result);
		else
			result[temp.j++] = input[temp.i++];
	}
	result[temp.j] = '\0';
	return (result);
}

void	expand_part(t_shell *shell)
{
	t_simple_cmds	*current_command;
	char			**str_array;
	char			*str;
	char			*new_str;

	current_command = shell->commands;
	while (current_command)
	{
		str_array = current_command->str;
		while (str_array && *str_array)
		{
			str = *str_array;
			if (str && *str)
			{
				new_str = process_str(str, shell);
				if (new_str)
				{
					free(str);
					*str_array = new_str;
				}
			}
			str_array++;
		}
		current_command = current_command->next;
	}
}
