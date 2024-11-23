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

char	*get_env_value(const char *var_name, char **envp)
{
	size_t	var_len;
	int		i;

	var_len = strlen(var_name);
	i = 0;
	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], var_name, var_len) == 0 && envp[i][var_len] == '=')
			return (&envp[i][var_len + 1]);
		i++;
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
	value = get_env_value(key, shell->envp);
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
