/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:38 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 11:55:47 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_listmap(t_list **list)
{
	t_list	*last_node;
	t_list	*new_node;
	int		i;
	int		j;
	char	*buffer;

	if (!list || !(*list))
		return (0);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	new_node = malloc(sizeof(t_list));
	if (!new_node || !buffer)
		return (ft_listclear(NULL, new_node, buffer));
	last_node = ft_find_first_node(*list);
	i = 0;
	j = 0;
	while (last_node->str_buffer[i] && last_node->str_buffer[i] != '\n')
		++i;
	while (last_node->str_buffer[i] && last_node->str_buffer[++i])
		buffer[j++] = last_node->str_buffer[i];
	buffer[j] = '\0';
	new_node->str_buffer = buffer;
	new_node->next = NULL;
	ft_listclear(list, new_node, buffer);
	return (1);
}

char	*ft_read_line(t_list *list)
{
	size_t	str_len;
	char	*next_str;

	if (!list)
		return (NULL);
	str_len = ft_length_to_newline(list);
	next_str = malloc(str_len + 1);
	if (!next_str)
		return (NULL);
	ft_strcpy_fromlist(list, next_str);
	return (next_str);
}

void	ft_listadd_back(t_list **list, char *buffer)
{
	t_list	*new_node;
	t_list	*last_node;

	last_node = ft_find_first_node(*list);
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(buffer);
		return ;
	}
	if (!last_node)
		*list = new_node;
	else
		last_node->next = new_node;
	new_node->str_buffer = buffer;
	new_node->next = NULL;
}

int	ft_create_list(t_list **list, int fd)
{
	int		bytes_read;	
	char	*buffer;

	while (!ft_is_found_newline(*list))
	{
		buffer = malloc((size_t)BUFFER_SIZE + 1);
		if (!buffer)
			return (0);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(buffer);
			if (bytes_read == -1)
				return (-1);
			return (0);
		}
		buffer[bytes_read] = '\0';
		ft_listadd_back(list, buffer);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	char			*next_line;
	int				exit;

	if (fd < 0 || BUFFER_SIZE <= 0 || \
	INT_MAX < BUFFER_SIZE || fd > OPEN_MAX)
		return (NULL);
	exit = 1;
	exit = ft_create_list(&list, fd);
	if (exit == -1)
	{
		ft_listclear(&list, NULL, NULL);
		return (NULL);
	}
	next_line = ft_read_line(list);
	exit = 1;
	exit = ft_listmap(&list);
	if (!exit)
	{
		ft_listclear(&list, NULL, NULL);
		return (NULL);
	}
	return (next_line);
}