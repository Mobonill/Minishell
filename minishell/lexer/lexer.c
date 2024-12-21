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

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	shell->env = init_env((const char **)envp, shell);
	return (shell);
}

int	handle_input(t_shell *shell)
{
	ft_init_shell_1(shell);
	shell->input_line = readline(GREEN "Minishell$ " RESET);
	if (shell->input_line == NULL)
	{
		free_env(shell->env);
		free(shell);
		ft_signal_ctr_d();
		return (0);
	}
	if (ft_strlen(shell->input_line) > 0)
	{
		add_history(shell->input_line);
		return (1);
	}
	free(shell->input_line);
	shell->input_line = NULL;
	return (0);
}

void	process_input(t_shell *shell)
{
	if (ft_minicheck(shell->input_line))
	{
		g_global_exit = 2;
		ft_free_lex(shell);
		return ;
	}
	if (!check_quotes(shell->input_line))
	{
		ft_fprintf(2, "bash: unmatched quotes found\n");
		g_global_exit = 2;
		ft_free_lex(shell);
		return ;
	}
	ft_read_token(shell->input_line, &shell->lexer_list, shell);
	if (ft_check_errors(shell->lexer_list))
	{
		g_global_exit = 2;
		ft_free_lex(shell);
		return ;
	}
	parser_part(shell->count_pipe, &shell->lexer_list, shell);
	expand_part(shell);
	g_global_exit = execute_minishell(shell, shell->commands);
	ft_free_tous(shell);
}

void	ft_start_loop(char **envp)
{
	t_shell	*shell;

	shell = init_shell(envp);
	while (1)
	{
		if (!handle_input(shell))
			continue ;
		process_input(shell);
	}
	free_env(shell->env);
	free(shell);
}
