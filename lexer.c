/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 19:38:52 by mobonill          #+#    #+#             */
/*   Updated: 2024/10/28 17:25:47 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// echo "Bonjour" | echo "Salut" > file.txt

// LEXER ---> [echo] --> [Bonjour] ---> [ | ] ---> [echo] ---> [Salut] ---> [ > ] ----> [file.txt]
//				0	 next	 0		next  1			  0			  0			  2				0


// typedef struct s_lexer
// {
// 	char	*str;
// 	int		token;

// }	t_lexer;


#include "lexer.h"


// int	is_token(char lexer)
// {
// 	if (lexer != 0)
// 		return (1);
// 	return (0);
// }

// int	lexer(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (input[i] >= 7 && input[i] <= 32) // to manage whitespaces
// 		i++;
	

// 	return ();
// }

// void	signals_handler() // to manage ctrl C ctrl D
// {
// 	signal(SIGINT, sigint);
// 	signal(SIGQUIT, SIG_IGN);
// }



void	minishell_loop()
{
	char *input;
	char **lexer;
	t_list	*new;
	t_list	**lst;
	int		i;

	input = NULL;
	i = 0;
	while(1)
	{
		input = readline("Minishell>");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		add_history(input);

	}
	rl_clear_history();
	free(input);
	// free(lexer);
}

int	main()
{
	minishell_loop();
	return (0);
}