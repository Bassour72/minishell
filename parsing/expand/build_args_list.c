/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_args_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:46:53 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 22:48:51 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	move_the_loop(t_expand_node **tmp)
{
	t_expand_node	*tmp_node;

	tmp_node = *tmp;
	*tmp = (*tmp)->next;
	free(tmp_node->data);
	free(tmp_node);
}

int	join_joinable_nodes(t_expand_node *list, t_expand_node **current)
{
	t_expand_node	*tmp;
	char			*tmp_data;

	tmp = (*current)->next;
	while (tmp)
	{
		tmp_data = ft_strjoin((*current)->data, tmp->data);
		if (!tmp_data)
			return (perror("error: "), R_FAIL);
		free((*current)->data);
		(*current)->data = tmp_data;
		(*current)->next = tmp->next;
		if (tmp->joinable == 0)
		{
			move_the_loop(&tmp);
			break ;
		}
		move_the_loop(&tmp);
	}
	*current = tmp;
	return (R_SUCCESS);
}

int	build_args_list_from_nodes_by_joining(t_expand_node *list)
{
	t_expand_node	*tmp;
	t_expand_node	*current;
	t_expand_node	*tmp_node;
	char			*tmp_data;

	current = list;
	while (current)
	{
		if (current->joinable == 1)
		{
			if (join_joinable_nodes(list, &current) == R_FAIL)
			{
				return (R_FAIL);
			}
		}
		else
		{
			current = current->next;
		}
	}
	return (R_SUCCESS);
}
