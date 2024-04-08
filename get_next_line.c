/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsilva-m <jsilva-m@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 17:37:21 by jsilva-m          #+#    #+#             */
/*   Updated: 2024/03/18 09:43:33 by jsilva-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_buffer(int fd, char **stash, char *buffer)
{
	char	*tmp;
	int		bytes;

	ft_bzero(buffer, BUFFER_SIZE + 1);
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (bytes < 0 || buffer == NULL)
	{
		free(*stash);
		*stash = NULL;
		return (-1);
	}
	if (bytes == 0)
		return (bytes);
	tmp = ft_strjoin(*stash, buffer);
	if (tmp == NULL)
	{
		free(*stash);
		*stash = NULL;
		return (-1);
	}
	free(*stash);
	*stash = tmp;
	return (bytes);
}

static void	remove_result(char **stash)
{
	char	*nl;
	char	*tmp;
	size_t	i;
	size_t	j;

	nl = ft_strchr(*stash, '\n');
	if (!nl)
	{
		free(*stash);
		*stash = NULL;
		return ;
	}
	tmp = malloc((ft_strlen(nl)) * sizeof(char));
	i = 0;
	j = ft_strlen(*stash) - ft_strlen(nl) + 1;
	while (j < ft_strlen(*stash))
		tmp[i++] = (*stash)[j++];
	tmp[i] = '\0';
	free(*stash);
	*stash = tmp;
	if (**stash == 0)
	{
		free(*stash);
		*stash = NULL;
	}
}

static void	get_result(char **stash, char **result)
{
	char	*nl;
	size_t	len;
	size_t	i;

	nl = ft_strchr(*stash, '\n');
	len = ft_strlen(*stash) - ft_strlen(nl) + 2;
	*result = (char *)malloc(len * sizeof(char));
	if (!result)
		return ;
	i = 0;
	while (i < len - 1)
	{
		(*result)[i] = (*stash)[i];
		i++;
	}
	(*result)[i] = '\0';
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*result;
	char		*buffer;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	bytes = 1;
	while (ft_strchr(stash, '\n') == NULL && bytes > 0)
		bytes = read_buffer(fd, &stash, buffer);
	free(buffer);
	if (bytes == -1)
		return (NULL);
	if (ft_strlen(stash) == 0)
		return (NULL);
	get_result(&stash, &result);
	remove_result(&stash);
	return (result);
}
/*
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error al abrir el archivo\n");
        return (1);
    }
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }
    close(fd);
    return (0);
}
*/