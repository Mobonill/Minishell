/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 15:35:59 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/22 15:36:01 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_env_value(const char *var_name, t_env *env)
{
	t_env	*current;

	if (!var_name || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (current->name && ft_strcmp(current->name, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	handle_exit_status(char *result, t_temp *temp)
{
	char	*exit_code;
	char	*ptr;

	exit_code = ft_itoa(g_global_exit);
	if (!exit_code)
		return ;
	ptr = exit_code;
	while (*ptr)
		result[temp->j++] = *ptr++;
	free(exit_code);
	temp->i++;
}

void	han_env1(t_shell *shell, const char *str, t_temp *temp, char *result)
{
	int		start;
	int		k;
	char	*key;
	char	*value;

	start = temp->i;
	k = 0;
	while (isalnum(str[temp->i]) || str[temp->i] == '_')
		temp->i++;
	key = ft_substr(str, start, temp->i - start);
	value = get_env_value(key, shell->env);
	if (value)
	{
		while (value[k])
			result[temp->j++] = value[k++];
	}
	free(key);
}

void	handle_dollar(t_shell *shell, const char *str,
			t_temp *temp, char *result)
{
	temp->i++;
	if (str[temp->i] == '\0' || ft_ifspace(str[temp->i]))
		result[temp->j++] = '$';
	else if (str[temp->i] == '?')
		handle_exit_status(result, temp);
	else if (isdigit(str[temp->i]))
	{
		temp->i++;
		while (isdigit(str[temp->i]))
			result[temp->j++] = str[temp->i++];
	}
	else if (isalpha(str[temp->i]) || str[temp->i] == '_')
		han_env1(shell, str, temp, result);
	else
		temp->i++;
}
