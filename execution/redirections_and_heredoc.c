/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_and_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/06 16:11:32 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirections(t_exec *exec, t_simple_cmds *parser)
{
	t_lexer	*redir;

	redir = parser->redirections;
	while (redir != NULL)
	{
		if (redir->token == IN)
		{
			exec->input = open(redir->str, O_RDONLY);
			if (exec->input < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->input, STDIN_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->input), -1);
			close(exec->input);
			exec->input = -1;
		}
		else if (redir->token == OUT || redir->token == APPEND)
		{
			if (redir->token == OUT)
				exec->output = open(redir->str, O_WRONLY | O_CREAT, 0644);
			else
				exec->output = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (exec->output < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->output, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->output), -1);
			close(exec->output);
			exec->output = -1;
		}
		else if(redir->token == HEREDOC)
		{
			exec->input = ft_handle_heredoc(redir->str);
			if (exec->input < 0)
				return (-1);
			if (dup2(exec->input, STDIN_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->input), -1);
			close(exec->input);
			exec->input = -1;
		}
		redir = redir->next;
	}
	return (0);
}

int	ft_handle_heredoc(char *str)
{
	char	*line;
	char	*name;
	int		tmp_fd;
	int		index;

	line = NULL;
	index = 1;
	name = generate_heredoc_filename(index);
	heredoc_signals();
	tmp_fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd < 0)
	{
		perror("");
		return(-1);
	}
	while (1)
	{
		line = readline(">");
		if (!line || ft_strcmp(line, str) == 0)
			break; 
		ft_fprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(tmp_fd);
	tmp_fd = open(name, O_RDONLY);
	if (tmp_fd < 0)
	{
		perror("");
		return (-1);
	}
	// reset_signals();
	index++;
	return (tmp_fd);
}
