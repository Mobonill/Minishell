/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:28:14 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/17 18:45:14 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
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
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_signal_ctr_c);
}

