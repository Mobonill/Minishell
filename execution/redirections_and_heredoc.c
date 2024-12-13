/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_and_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/13 19:09:32 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirections(t_exec *exec, t_simple_cmds *parser)
{
	t_lexer	*redir;
	int		fd_heredoc;
	int		i;
	
	i = -1;
	exec->heredoc_index = 0;
	redir = parser->redirections;
	while (redir != NULL)
	{
		if (redir->token == IN)
		{
			exec->input = open(redir->str, O_RDONLY);
			if (exec->input < 0)
				return (perror(redir->str), -1);
			if (dup2(exec->input, STDIN_FILENO) < 0)
				return (perror(""), close(exec->input), -1);
			close(exec->input);
			// printf("cmd = %s fd in = %d \n", parser->str[0], exec->input);
			if (exec->input != -1)
				close(exec->input);
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
				return (perror(""), close(exec->output), -1);
			// printf("cmd = %s fd out = %d \n", parser->str[0], exec->output);
		}
		else if (redir->token == HEREDOC)
		{
			if (exec->num_heredoc == 0)
			{
				exec->num_heredoc = count_heredocs(parser->redirections);
				exec->heredoc_fd = malloc(sizeof(int) * exec->num_heredoc);
				if (!exec->heredoc_fd)
				{
					perror("");
					return (-1);
				}
				while (++i < exec->num_heredoc)
					exec->heredoc_fd[i] = -1;
			}
			fd_heredoc = ft_handle_heredoc(redir->str, exec->heredoc_index);
			if (fd_heredoc < 0)
				return (-1);
			exec->heredoc_fd[exec->heredoc_index] = fd_heredoc;
			exec->heredoc_index++;
		}
		redir = redir->next;
	}
	if (!parser || !parser->str || !parser->str[0])
	{
		cleanup_heredoc_files(exec);
		return (0);
	}
	return (0);
}

int	ft_handle_heredoc(char *str, int index)
{
	char	*line;
	char	*name;
	int		tmp_fd;
	
	line = NULL;
	name = generate_heredoc_filename(index);
	if (!name)
		return(-1);
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
		free(name);
		perror("");
		return (-1);
	}
	free(name);
	return (tmp_fd);
}
