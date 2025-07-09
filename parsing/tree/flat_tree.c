/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flat_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:33:46 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 14:05:19 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

t_tree	*new_tree_node(t_type type)
{
	t_tree	*tree_node;

	tree_node = malloc(sizeof(t_tree));
	if (!tree_node)
		return (perror("error"), NULL);
	tree_node->data = NULL;
	tree_node->redirections = NULL;
	tree_node->type = type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->empty = 0;
	return (tree_node);
}

static int	operators(t_tree **tree_node, t_token *token, int *i)
{
	*tree_node = new_tree_node(token[*i].type);
	if (!*tree_node)
		return (R_FAIL);
	(*i)++;
	return (R_SUCCESS);
}

static int	normal_block(t_flat_tree *flat_tree_list, t_tree **tree_node, \
t_token *token, int *i)
{
	if (skip_listed_tokens(token, i))
		return (R_CONTINUE);
	*tree_node = new_tree_node(BLOCK);
	if (!*tree_node)
		return (free_flat_tree(flat_tree_list), R_FAIL);
	if (!collect_reds_and_cmds(token, i, *tree_node, flat_tree_list))
		return (R_FAIL);
	return (R_SUCCESS);
}

static int	create_special_blocks(t_flat_tree *flat_tree_list, \
t_tree **tree_node, t_token *token, int *i)
{
	if ((token[*i].type == PIPE || token[*i].type == OP_AND || \
	token[*i].type == OP_OR))
	{
		if (operators(tree_node, token, i) == R_FAIL)
			return (free_flat_tree(flat_tree_list), 0);
		return (R_SUCCESS);
	}
	else if (token[*i].type == PAREN_CLOSE)
	{
		if (close_parenths_block(tree_node, i) == R_FAIL)
			return (free_flat_tree(flat_tree_list), 0);
		return (R_SUCCESS);
	}
	else if (token[*i].type == PAREN_OPEN)
	{
		if (open_parenths_block(token, tree_node, i) == R_FAIL)
			return (free_flat_tree(flat_tree_list), 0);
		return (R_SUCCESS);
	}
	return (R_CONTINUE);
}

t_flat_tree	*create_flat_tree(t_token *token)
{
	t_flat_tree	*flat_tree_list;
	t_tree		*tree_node;
	int			i;
	int			state;

	flat_tree_list = NULL;
	i = 0;
	while (token[i].data)
	{
		state = create_special_blocks(flat_tree_list, &tree_node, token, &i);
		if (state == R_FAIL)
			return (R_FAIL);
		else if (state == R_CONTINUE)
		{
			state = normal_block(flat_tree_list, &tree_node, token, &i);
			if (state == R_CONTINUE)
				continue ;
			else if (state == R_FAIL)
				return (R_FAIL);
		}
		if (append_new_flat_tree_node(&flat_tree_list, tree_node) == R_FAIL)
			return (NULL);
	}
	return (flat_tree_list);
}
