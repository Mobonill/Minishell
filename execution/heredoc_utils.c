/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:50:14 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/13 18:34:40 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*generate_heredoc_filename(int index)
{
	char	*filename;
	char	*index_str;

	filename = malloc(40);
	// if (!filename)
	// {
	// 	perror("malloc failed");
	// 	return (1);
	// }
	index_str = ft_itoa(index);
	// if (!index_str)
	// {
	// 	perror("itoa failed");
	// 	free(filename);
	// 	return (1);
	// }
	ft_strlcpy(filename, ".heredoc_tmp_", 40);
	ft_strlcat(filename, index_str, 40);
	free(index_str);
	return (filename);
}
void	cleanup_heredoc_files(t_exec *exec)
{
	char	*filename;
	int		i;

	i = -1;
	if (!exec->heredoc_fd)
		return;
	while(++i < exec->num_heredoc)
	{
		if (exec->heredoc_fd[i] != -1)
		{
			close(exec->heredoc_fd[i]);
			exec->heredoc_fd[i] = -1;
		}
		filename = generate_heredoc_filename(i);
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
	}
	free(exec->heredoc_fd);
	exec->heredoc_fd = NULL;
	exec->num_heredoc = 0;
}

int	count_heredocs(t_lexer *redirections)
{
	int count;

	count = 0;
	while (redirections)
	{
		if (redirections->token == HEREDOC)
			count++;
		redirections = redirections->next;
	}
	return (count);
}
