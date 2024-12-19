/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:38:14 by zserobia          #+#    #+#             */
/*   Updated: 2024/12/06 16:39:06 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_all_digits(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_simple_cmds *simple_cmd, t_shell *shell)
{
    if (!simple_cmd->str[1])
    {
        printf("exit\n");
        g_global_exit = 0;
    }
	else if (!is_all_digits(simple_cmd->str[1]))
	{
		printf("exit\nbash: exit: %s: numeric argument required\n",
			simple_cmd->str[1]);
		g_global_exit = 2;
	}
	else if (simple_cmd->str[2])
	{
		printf("exit\nbash: exit: too many arguments\n");
		g_global_exit = 1;
		return (1);
	}
	else
	{
		g_global_exit = ft_atoi(simple_cmd->str[1]) % 256;
		if (g_global_exit < 0)
			g_global_exit = 256 + g_global_exit;
	}
	ft_free_tous(shell);
	printf("status exit  = %d\n", g_global_exit);
	exit(g_global_exit);
}
