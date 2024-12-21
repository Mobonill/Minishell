/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_and_heredoc.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:17:47 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/21 18:51:23 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirections_and_heredoc(t_exec *exec, t_simple_cmds *parser, int i)
{
	exec->heredoc_index = 0;
	if (handle_redirections(exec, parser, parser->redirections) != 0)
		return (g_global_exit);
	if (last_heredoc_is_redirected(exec) != 0)
		return (g_global_exit);
	if (exec->num_heredoc > 0 && i < exec->num_heredoc)
	{
		if (exec->heredoc_fd[i] != -1)
		{
			close(exec->heredoc_fd[i]);
			exec->heredoc_fd[i] = -1;
		}
	}
	if ((!parser || !parser->str || !parser->str[0]) && exec->num_heredoc)
		return (cleanup_heredoc_files(exec), g_global_exit);
	return (0);
}

int	handle_redirections(t_exec *exec, t_simple_cmds *parser, t_lexer *redir)
{
	while (redir != NULL)
	{
		if (redir->token == HEREDOC
			&& handle_heredoc_redirection(exec, parser, redir) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = parser->redirections;
	while (redir != NULL)
	{
		if (redir->token == IN && handle_in_redirection(exec, redir) < 0)
			return (-1);
		redir = redir->next;
	}
	redir = parser->redirections;
	while (redir != NULL)
	{
		if (redir->token == OUT || redir->token == APPEND)
		{
			if (handle_out_redirection(exec, redir) < 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	handle_heredoc_redirection(t_exec *exec, t_simple_cmds *parser,
	t_lexer *redir)
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

int	handle_in_redirection(t_exec *exec, t_lexer *redir)
{
	exec->input = open(redir->str, O_RDONLY);
	if (exec->input < 0)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		perror(redir->str);
		g_global_exit = 1;
		exit (1);
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
		ft_putstr_fd("bash: ", STDERR_FILENO);
		perror(redir->str);
		g_global_exit = 1;
		exit (1);
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
