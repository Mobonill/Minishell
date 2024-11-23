<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:25:34 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/20 16:26:36 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"

void	free_pipes(t_exec *exec)
{
	int i;

	i = 0;
	if (exec->fd)
	{
		i = 0;
		while (i <= exec->num_pipes + 2) // pour les redirections
		{
			free(exec->fd[i]);
			i++;
		}
		free(exec->fd);
	}
}
void	free_all(t_exec *exec)
{
	free_pipes(exec);
	free(exec->pid);
}

void	free_cmd_argv(t_simple_cmds *parser)
{
	int i;

	i = 0;
	while (parser->str[i])
	{
		free(parser->str[i]);
		i++;
	}
	free(parser->str);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:01:56 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/13 12:01:59 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

void	ft_free_lex(t_shell *shell)
{
	if (shell->input_line)
	{
		free(shell->input_line);
		shell->input_line = NULL;
	}
	if (shell->lexer_list)
		free_lexer_list(&shell->lexer_list);
}

void	free_lexer_list(t_lexer **lexer)
{
	t_lexer	*current;
	t_lexer	*next;

	current = *lexer;
	if (lexer == NULL || *lexer == NULL)
		return ;
	while (current)
	{
		next = current->next;
		if (current->str)
			free(current->str);
		free(current);
		current = next;
	}
	*lexer = NULL;
}

void	free_redirections(t_lexer *redirection)
{
	t_lexer	*current;
	t_lexer	*next;

	current = redirection;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
}

void	free_simple_cmds_list(t_simple_cmds *commands)
{
	t_simple_cmds	*current;
	int				i;

	while (commands)
	{
		current = commands;
		commands = commands->next;
		if (current->str)
		{
			i = 0;
			while (current->str[i])
			{
				free(current->str[i]);
				current->str[i] = NULL;
				i++;
			}
			free(current->str);
			current->str = NULL;
		}
		free_redirections(current->redirections);
		current->redirections = NULL;
		free(current);
	}
}
>>>>>>> lexer
