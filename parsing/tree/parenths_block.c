/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenths_block.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:27:15 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:33:22 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	close_parenths_block(t_tree **tree_node, int *i)
{
	*tree_node = new_tree_node(BLOCK);
	if (!*tree_node)
		return (R_FAIL);
	(*tree_node)->empty = -1;
	(*i)++;
	return (R_SUCCESS);
}

int	open_parenths_block(t_token *token, t_tree **tree_node, int *i)
{
	*tree_node = new_tree_node(BLOCK);
	if (!*tree_node)
		return (R_FAIL);
	(*tree_node)->empty = 1;
	if (parenths_redirections(*tree_node, &token[*i]) == R_FAIL)
		return (R_FAIL);
	(*i)++;
	return (R_SUCCESS);
}
