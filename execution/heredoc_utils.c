/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:50:14 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/06 15:34:12 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*generate_heredoc_filename(int index)
{
	char	*filename;
	char	*index_str;

	filename = malloc(40);
	if (!filename)
	{
		perror("");
		exit(1);
	}
	index_str = ft_itoa(index);
	ft_strlcpy(filename, ".heredoc_tmp_", 14);
	ft_strlcat(filename, index_str, 14);
	return (filename);
}