/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 01:20:40 by mobonill          #+#    #+#             */
/*   Updated: 2024/08/29 23:54:18 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_my_dest(char *rest)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!rest)
		return (NULL);
	while (rest[i] && rest[i] != '\n')
		i++;
	tmp = (char *)malloc(sizeof(char) * (i + 2));
	if (!tmp)
		return (NULL);
	i = 0;
	while (rest[i] && rest[i] != '\n')
	{
		tmp[i] = rest[i];
		i++;
	}
	if (rest[i] && rest[i] == '\n')
	{
		tmp[i] = rest[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*get_my_rest(char *rest)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	while (rest[i] && rest[i] != '\n')
		i++;
	if (!rest[i])
	{
		free(rest);
		return (NULL);
	}
	tmp = malloc(sizeof(char) * ft_strlen_gnl(rest) - i + 1);
	if (!tmp)
		return (NULL);
	i++;
	j = 0;
	while (rest[i])
		tmp[j++] = rest[i++];
	tmp[j] = '\0';
	free(rest);
	return (tmp);
}

char	*get_next_line(int fd)
{
	char		*dest;
	static char	*rest;
	char		buffer[BUFFER_SIZE + 1];
	int			bytes_reader;

	dest = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes_reader = 1;
	while (bytes_reader > 0 && !ft_strchr_gnl(rest, '\n'))
	{
		bytes_reader = read(fd, buffer, BUFFER_SIZE);
		if (bytes_reader < 0)
			return (NULL);
		buffer[bytes_reader] = '\0';
		rest = ft_strjoin_gnl(rest, buffer);
		if (!rest)
			return (NULL);
	}
	dest = get_my_dest(rest);
	rest = get_my_rest(rest);
	if (ft_strlen_gnl(dest) == 0)
		return (free(dest), NULL);
	return (dest);
}

// int	main(int argc, char **argv)
// {
// 	int fd;
// 	char *line;

// 	if (argc < 2)
// 	{
// 		argv[1] = "";
// 	}
// 	fd = open("fd.txt", O_RDONLY);
// 	for (int i = 0; i < atoi(argv[1]); ++i)
// 	{
// 		line = get_next_line(fd);
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }