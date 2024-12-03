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

t_shell	*ft_init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->input_line = NULL;
	shell->lexer_list = NULL;
	shell->count_pipe = 0;
	shell->pars = NULL;
	shell->envp = envp;
	shell->commands = NULL;
	signals();
	return (shell);
}



void	ft_start_loop(char **envp)
{
	t_shell	*shell;

	shell = NULL;
	shell = ft_init_shell(envp);
	if (!shell)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		shell->input_line = readline("Minishell$ ");
		// if (shell->input_line == NULL)
		// 	ft_signal_ctr_d();
		if (shell->input_line == NULL)
		{
<<<<<<< HEAD
			// Si readline retourne NULL (EOF), on quitte proprement
			printf("exit\n");
			// ft_free_tous(shell);
			break;
=======
			printf("problem signals\n");
			ft_signal_ctr_d();
>>>>>>> 0bd304f8008a01d3002fcf47d9d3ba53cecd8af4
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
				printf("-bash: Error: unmatched quotes found.\n");
				ft_free_lex(shell);
				continue ;
			}
			ft_read_token(shell->input_line, &shell->lexer_list, shell);
			if (ft_check_errors(shell->lexer_list))
			{
				ft_free_lex(shell);
				continue ;
			}
			print_tokens(shell->lexer_list);
			parser_part(shell->count_pipe, shell->lexer_list, shell);
			printf("AVANT\n");
			print_simple_cmds(shell->commands);
			expand_part(shell);
			printf("APRES\n");
			print_simple_cmds(shell->commands);
			shell->env = init_env((const char **)envp, shell);
<<<<<<< HEAD
			if(execute_minishell(shell, shell->commands) != 0)
			{
				fprintf(stderr, "Execution failed\n");
				// ft_free_tous(shell);
				continue;
			}
=======
			execute_minishell(shell, shell->commands);
			reset_signals();
			// ft_free_tous(shell);
>>>>>>> 0bd304f8008a01d3002fcf47d9d3ba53cecd8af4
		}
		ft_free_tous(shell);
	}
	free_env(shell->env);
	free(shell);
}

