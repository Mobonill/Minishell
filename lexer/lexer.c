/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:00:47 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:00:49 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_global_exit = 0;

void	ft_init_shell_1(t_shell *shell)
{
	shell->input_line = NULL;
	shell->lexer_list = NULL;
	shell->count_pipe = 0;
	shell->pars = NULL;
	shell->commands = NULL;
	signals();
	return ;
}


void	ft_start_loop(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	shell->env = init_env((const char **)envp, shell);
	while (1)
	{
		ft_init_shell_1(shell);
		shell->input_line = readline("Minishell$ ");
		if (shell->input_line == NULL)
		{
			free_env(shell->env);
			free(shell);
			ft_signal_ctr_d();
		}
		if (ft_strlen(shell->input_line) > 0)
		{
			add_history(shell->input_line);
			if (ft_minicheck(shell->input_line))
			{
				ft_free_lex(shell);
				continue ;
			}
			if (!check_quotes(shell->input_line))
			{
				//printf("-bash: Error: unmatched quotes found.\n");
				ft_free_lex(shell);
				continue ;
			}
			ft_read_token(shell->input_line, &shell->lexer_list, shell);
			if (ft_check_errors(shell->lexer_list))
			{
				ft_free_lex(shell);
				continue ;
			}
			// print_tokens(shell->lexer_list);
			parser_part(shell->count_pipe, &shell->lexer_list,shell);
			// printf("AVANT\n");
			// print_simple_cmds(shell->commands);
			expand_part(shell);
			// printf("APRES\n");
			// print_simple_cmds(shell->commands);
			execute_minishell(shell, shell->commands);
			ft_printf("Last exit status: %d\n", g_global_exit);
			ft_free_tous(shell);
		}
		else
		{
			free(shell->input_line);
			shell->input_line = NULL;
		}
	}
	free_env(shell->env);
	free(shell);
}

