/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_and_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/11/27 16:06:48 by mobonill         ###   ########.fr       */
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
		}
		else if (redir->token == OUT || redir->token == APPEND)
		{
			exec->output = open(redir->str,
				O_WRONLY | O_CREAT | (redir->token == OUT ? O_TRUNC : O_APPEND),
					0644);
			if (exec->output < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->output, STDOUT_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->output), -1);
			close(exec->output);
		}
		else if(redir->token == HEREDOC)
		{
			exec->input = ft_handle_heredoc(redir->str);
			printf("exec = %d", exec->input);
			if (exec->input < 0)
				return (-1);
			if (dup2(exec->input, STDIN_FILENO) < 0)
				return (perror("dup2 failed"), close(exec->input), -1);
			close(exec->input);
		}
		redir = redir->next;
	}
	// unlink(".heredoc_tmp");
	return (0);
}

int	ft_handle_heredoc(char *str)
{
	char	*line;
	int		tmp_fd;

	line = NULL;
	tmp_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd < 0)
	{
		perror("heredoc");
		exit(1);
	}
	while (1)
	{
		line = readline(">");
		heredoc_signals();
		if (!line || ft_strcmp(line, str) == 0)
			break;
		ft_fprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(tmp_fd);
	tmp_fd = open(".heredoc_tmp", O_RDONLY);
	if (tmp_fd < 0)
	{
		perror("dup2 failed");
		return (-1);
	}
	return (tmp_fd);
}
