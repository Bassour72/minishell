/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flat_tree_collectors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:34:21 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:24:07 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	collect_redirections(t_token *token, int i, t_tree *tree_node)
{
	if ((token[i].type == RED_INPUT || \
		token[i].type == HER_DOC || \
		token[i].type == RED_APPEND || \
		token[i].type == RED_TRUNK) && \
		token[i].is_listed == 0)
	{
		token[i].is_listed = 1;
		if (!new_red(tree_node, token[i].type, token[i + 1].data))
			return (R_FAIL);
	}
	return (R_SUCCESS);
}

int	collect_words(t_token *token, int i, t_tree *tree_node)
{
	if (token[i].type == WORD)
	{
		tree_node->data = append_command(tree_node->data, token[i].data);
		if (!tree_node->data)
			return (free_tree_node(tree_node), 0);
	}
	return (1);
}

int	collect_reds_and_cmds(t_token *token, int *i, t_tree *tree_node, \
t_flat_tree *flat_tree_list)
{
	while (token[*i].data && token[*i].type != PIPE && \
	token[*i].type != OP_AND && token[*i].type != OP_OR && \
	token[*i].type != PAREN_CLOSE)
	{
		if (token[*i].type == WORD)
		{
			if (!collect_words(token, *i, tree_node))
				return (free_flat_tree(flat_tree_list), 0);
		}
		else
		{
			if (!collect_redirections(token, *i, tree_node))
				return (free_flat_tree(flat_tree_list), 0);
		}
		(*i)++;
	}
	return (1);
}
