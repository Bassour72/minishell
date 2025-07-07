/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:47:49 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 22:17:53 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

int	get_wc_list_len(t_wc_node *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

void	cleanup_wc_list_shels(t_wc_node *list)
{
	t_wc_node	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

void	assign_wc_node_data_to_args_array(t_wc_node *list, char ***arr)
{
	int	i;

	i = 0;
	while (list)
	{
		(*arr)[i++] = list->data;
		list = list->next;
	}
	(*arr)[i] = NULL;
}

void	free_wc_node_list(t_wc_node *list)
{
	t_wc_node	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}
