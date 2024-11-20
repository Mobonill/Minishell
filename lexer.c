/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:38:52 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/20 17:06:10 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"


void	minishell_loop(t_shell *shell)
{
	char *input;
	t_simple_cmds *parser;

	parser->num_redirections = 0;
	parser->redirections->token = IN;
	parser->redirections->str = "infile1";
	input = NULL;
	while (1)
	{
		input = readline("Minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		add_history(input);
		execute_minishell(shell, parser);
	}
	rl_clear_history();
	free(input);
}


int	main(int argc, char **av, const char **envp)
{
	(void)argc;
	(void)av;

	t_shell	*shell;

	shell = NULL;
	shell = init_env(envp, shell);
	minishell_loop(shell);
	free_env(shell->env);
	free(shell);
	return (0);
}