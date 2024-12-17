/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 16:26:52 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/17 18:19:45 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	g_global_exit = 0;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	ft_start_loop(envp);
	rl_clear_history();
	return (0);
}
