/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:38:52 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/05 18:40:37 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// echo "Bonjour" | echo "Salut" > file.txt

// LEXER ---> [echo] --> [Bonjour] ---> [ | ] ---> [echo] ---> [Salut] ---> [ > ] ----> [file.txt]
//				0	 next	 0		next  1			  0			  0			  2				0



#include "./include/lexer.h"



// void	signals_handler() // to manage ctrl C ctrl D
// {
// 	signal(SIGINT, sigint);
// 	signal(SIGQUIT, SIG_IGN);
// }

void	minishell_loop(t_env *env)
{
	char *input;
	// char **lexer;
	// t_list	*new;
	// t_list	**lst;
	// int		i;

	input = NULL;
	// i = 0;
	while(1)
	{
		input = readline("Minishell>");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		add_history(input);
		choose_exec("UNSET", env);
	}
	rl_clear_history();
	free(input);
	// free(lexer);
}

int	main(int argc, char **av, const char **envp)
{
	t_env	*env;

	if(av[0] != NULL && argc < 12)
	{
		env = init_env(envp);
		minishell_loop(env);
		free_env(env);
	}
	return (0);
}