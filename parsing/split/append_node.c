/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:52:09 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/02 10:14:00 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	creat_the_list_from_zero(t_node **list, char **data)
{
	*list = malloc(sizeof(t_node));
	if (!*list)
		return (perror("error: "), free(*data), R_FAIL);
	(*list)->next = NULL;
	(*list)->data = ft_strdup(*data);
	if (!(*list)->data)
		return (perror("error: "), free(*data), R_FAIL);
	free(*data);
	*data = NULL;
	return (R_SUCCESS);
}

static int	append_new_node_to_existen_list(t_node **list, char **data)
{
	t_node	*node;
	t_node	*tmp;

	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	node = malloc(sizeof(t_node));
	if (!node)
		return (perror("error: "), free(*data), R_FAIL);
	node->next = NULL;
	node->data = ft_strdup(*data);
	if (!node->data)
		return (perror("error: "), free(*data), free(node), R_FAIL);
	tmp->next = node;
	free(*data);
	*data = NULL;
	return (R_SUCCESS);
}

int	append_node(t_node **list, char **data)
{
	t_node	*node;

	if (!*list)
		return (creat_the_list_from_zero(list, data));
	else
		return (append_new_node_to_existen_list(list, data));
}
