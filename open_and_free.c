/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_and_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/12 19:45:28 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"


void	open_input(t_exec *exec, t_parser *parser)
{
	t_lexer	*cur;

	cur = parser->redirections;
	while (parser->redirections->str != IN_REDIR)
	{
		cur = cur->next;
	}
	exec->input = open(cur->str, O_RDONLY);
	if (exec->input < 0)
	{
		ft_fprintf(2, "bash: %s: %s", cur->str, perror);
		free(exec->pid);
		exit(1);
	}
}

void	open_output(t_exec *exec, t_parser *parser)
{
	t_lexer	*cur;

	cur = parser->redirections;
	while (parser->redirections->str != OUT_REDIR)
		cur = cur->next;
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
		while (i < exec->num_pipes + 2) // pour les redirections
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
