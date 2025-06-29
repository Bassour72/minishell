/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:49:22 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 22:49:54 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	append_new_list_node(t_node **list, char *str)
{
	t_node	*tmp;
	t_node	*new_node;
	char	*data;

	data = ft_strdup(str);
	if (!data)
		return (perror("error: "), R_FAIL);
	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (perror("error: "), free(data), R_FAIL);
	new_node->data = data;
	new_node->next = NULL;
	if (!*list)
		return ((*list) = new_node, R_SUCCESS);
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (R_SUCCESS);
}

static void	free_list(t_node *list)
{
	t_node	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

int	array_to_list(char **arr, t_node **list)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		if (!append_new_list_node(list, arr[i]))
			return (free_list(*list), R_FAIL);
	}
	return (R_SUCCESS);
}
