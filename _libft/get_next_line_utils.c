/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:23 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 11:56:33 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_is_found_newline(t_list *list)
{
	size_t	i;

	if (!list)
		return (0);
	while (list)
	{
		i = 0;
		while (list->str_buffer[i] && i < BUFFER_SIZE)
		{
			if (list->str_buffer[i] == '\n')
				return (1);
			++i;
		}
		list = list->next;
	}
	return (0);
}

t_list	*ft_find_first_node(t_list *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

void	ft_strcpy_fromlist(t_list *list, char *str)
{
	int	i;
	int	j;

	if (!list || !list->str_buffer)
		return ;
	j = 0;
	while (list)
	{
		i = 0;
		while (list->str_buffer[i])
		{
			if (list->str_buffer[i] == '\n')
			{
				str[j++] = '\n';
				str[j] = '\0';
				return ;
			}
			str[j++] = list->str_buffer[i];
			i++;
		}
		list = list->next;
	}
	str[j] = '\0';
}

size_t	ft_length_to_newline(t_list *list)
{
	size_t	i;
	size_t	len;

	if (!list)
		return (0);
	len = 0;
	while (list)
	{
		i = 0;
		while (list->str_buffer[i])
		{
			if (list->str_buffer[i] == '\n')
			{
				++len;
				return (len);
			}
			++i;
			++len;
		}
		list = list->next;
	}
	return (len);
}

int	ft_listclear(t_list **list, t_list *new_node, char *buffer)
{
	t_list	*tmp;

	if (!list || !(*list))
	{
		free(new_node);
		free(buffer);
		return (0);
	}
	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->str_buffer);
		free(*list);
		*list = tmp;
	}
	*list = NULL;
	if (new_node && new_node->str_buffer[0])
		*list = new_node;
	else
	{
		free(new_node);
		free(buffer);
	}
	return (1);
}