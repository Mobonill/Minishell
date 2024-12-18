/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_and_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/17 20:57:22 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_in_redirection(t_exec *exec, t_lexer *redir)
{
	exec->input = open(redir->str, O_RDONLY);
	if (exec->input < 0)
	{
		perror(redir->str);
		// cleanup_exec_resources(exec);
		g_global_exit = 1;
		return (-1);
	}
	if (dup2(exec->input, STDIN_FILENO) < 0)
	{
		perror("");
		close(exec->input);
		g_global_exit = 1;
		return (-1);
	}
	return (0);
}

int	handle_out_redirection(t_exec *exec, t_lexer *redir)
{
	if (redir->token == OUT)
		exec->output = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		exec->output = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (exec->output < 0)
	{
		perror(redir->str);
		// cleanup_exec_resources(exec);
		g_global_exit = 1;
		return (1);
	}
	if (dup2(exec->output, STDOUT_FILENO) < 0)
	{
		perror("");
		close(exec->output);
		g_global_exit = 1;
		return (-1);
	}
	return (0);
}

int	handle_heredoc_redirection(t_exec *exec, t_simple_cmds *parser, t_lexer *redir)
{
	int	fd_heredoc;
	int	i;

	if (exec->num_heredoc == 0)
	{
		exec->num_heredoc = count_heredocs(parser->redirections);
		exec->heredoc_fd = malloc(sizeof(int) * exec->num_heredoc);
		if (!exec->heredoc_fd)
			return (perror(""), -1);
		i = -1;
		while (++i < exec->num_heredoc)
			exec->heredoc_fd[i] = -1;
	}
	fd_heredoc = ft_handle_heredoc(redir->str, exec->heredoc_index);
	if (fd_heredoc < 0)
		return (-1);
	exec->heredoc_fd[exec->heredoc_index++] = fd_heredoc;
	return (0);
}

int	write_to_heredoc(const char *delimiter, const char *filename)
{
	char	*line;
	int		tmp_fd;

	line = NULL;
	tmp_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (tmp_fd < 0)
	{
		perror("");
		return (-1);
	}
	while (1)
	{
		line = readline(">");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break;
		ft_fprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(tmp_fd);
	return (0);
}

int	ft_handle_heredoc(char *str, int index)
{
	char	*name;
	int		tmp_fd;

	name = generate_heredoc_filename(index);
	if (!name)
		return (-1);
	heredoc_signals();
	if (write_to_heredoc(str, name) < 0)
	{
		free(name);
		return (-1);
	}
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

// int	handle_redirections(t_exec *exec, t_simple_cmds *parser)
// {
// 	t_lexer	*redir;

// 	exec->heredoc_index = 0;
// 	redir = parser->redirections;
// 	while (redir != NULL)
// 	{
// 		if (redir->token == IN && handle_in_redirection(exec, redir) < 0)
// 			return (-1);
// 		else if ((redir->token == OUT || redir->token == APPEND) &&
// 				 handle_out_redirection(exec, redir) < 0)
// 			return (-1);
// 		else if (redir->token == HEREDOC &&
// 				 handle_heredoc_redirection(exec, parser, redir) < 0)
// 			return (-1);
// 		redir = redir->next;
// 	}
// 	if (!parser || !parser->str || !parser->str[0])
// 		return (cleanup_heredoc_files(exec), 0);
// 	return (0);
// }
int handle_redirections(t_exec *exec, t_simple_cmds *parser)
{
	t_lexer *redir;

	exec->heredoc_index = 0;
	redir = parser->redirections;
	while (redir != NULL)
	{
		if (redir->token == HEREDOC)
		{
			if (handle_heredoc_redirection(exec, parser, redir) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	redir = parser->redirections; 
	while (redir != NULL)
	{
		if (redir->token == IN)
		{
			if (handle_in_redirection(exec, redir) < 0)
				return (-1);
		}
		else if (redir->token == OUT || redir->token == APPEND)
		{
			if (handle_out_redirection(exec, redir) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	if (!parser || !parser->str || !parser->str[0])
		return (cleanup_heredoc_files(exec), 0);
	return (0);
}
