/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:44:50 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/20 14:46:08 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
			break ;
		ft_fprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	free(line);
	close(tmp_fd);
	return (0);
}

int	last_heredoc_is_redirected(t_exec *exec)
{
	int	last_fd;
	int	i;

	i = -1;
	if (exec->num_heredoc > 0)
	{
		last_fd = exec->heredoc_fd[exec->num_heredoc - 1];
		if (last_fd >= 0)
		{
			if (dup2(last_fd, STDIN_FILENO) < 0)
			{
				perror("");
				return (close(last_fd), -1);
			}
		}
		while (++i < exec->num_heredoc - 1)
		{
			if (exec->heredoc_fd[i] != -1)
			{
				close(exec->heredoc_fd[i]);
				exec->heredoc_fd[i] = -1;
			}
		}
	}
	return (0);
}
