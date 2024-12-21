/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:06:53 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/19 17:06:55 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_signal_ctr_c);
}

void	ft_signal_ctr_c(int status)
{
	if (status == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_global_exit = 130;
	}
}

void	ft_signal_ctr_d(void)
{
	printf("exit\n");
	rl_clear_history();
	exit(0);
}

void	signals_heredoc(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_signal_ctr_c_hd);
}

void	ft_signal_ctr_c_hd(int status)
{
	if (status == SIGINT)
	{
		printf("\e[2K\r");
		exit(g_global_exit);
	}
}
