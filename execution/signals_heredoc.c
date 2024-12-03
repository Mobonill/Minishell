/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:28:14 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/28 10:43:57 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\nHeredoc interrupted\n", 21);
		g_global_exit = 1;
		close(STDIN_FILENO); 
	}
}

void	heredoc_signals(void)
{
	signal(SIGINT, signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
void	reset_signals(void)
{
	printf("reset signals heredoc\n");
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_signal_ctr_c);
}

// void	ft_signal_ctr_c_hd(int status)
// {
// 	if (status == SIGINT)
// 	{
// 		printf("\n");
// 		exit(1);
// 	}
// }
