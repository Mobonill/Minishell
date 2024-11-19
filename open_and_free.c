/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/18 17:28:36 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"


void	open_input(t_exec *exec, t_parser *parser)
{
	t_parser	*cur;

	cur = parser;
	exec->input = -1;
	while (cur != NULL)
	{
		if (cur->redirections->str == IN_REDIR)
		{
			if (exec->input != -1)
				close(exec->input);
			exec->input = open(cur->cmd[0], O_RDONLY);
			if (exec->input < 0)
			{
				ft_fprintf(2, "bash: %s: %s", cur->redirections->str, perror);
				free(exec->pid);
				exit(1);
			}
		}
		else if (cur->redirections->str == "DELIMITER")
		{
			if (exec->input != -1)
				close(exec->input);
			exec->input = ft_handle_heredoc(cur);
		}
		cur = cur->next;
	}
}

void	open_output(t_exec *exec, t_parser *parser)
{
	t_lexer	*cur;

	cur = parser->redirections;
	while (cur && parser->redirections->str != OUT_REDIR)
		cur = cur->next;
	// if (exec->heredoc != NULL)
	// 	exec->output = open(cur->str, O_APPEND);
	exec->output = open(cur->str, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (exec->output < 0)
	{
		ft_fprintf(2, "bash: %s: %s", cur->str, perror);
		free_pipes(exec);
		free(exec->pid);
		exit(1);
	}
}
void	free_pipes(t_exec *exec)
{
	int	i;

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

void	free_cmd_argv(t_parser *parser)
{
	int	i;

	i = 0;
	while (parser->cmd[i])
	{
		free(parser->cmd[i]);
		i++;
	}
	free(parser->cmd);
}
