/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:38:52 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/08 16:20:07 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"


void	minishell_loop(t_shell *shell)
{
	char *input;

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
		my_choosen_exec("EXPORT", shell);
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