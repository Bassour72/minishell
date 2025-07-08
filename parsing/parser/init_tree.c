/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:07:32 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 16:14:55 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	skip_parenths(t_flat_tree *flat, int *p)
{
	if (flat->tree_node->empty == -1)
		(*p)++;
	else if (flat->tree_node->empty == 1)
		(*p)--;
}

static t_tree	*collect_parenths(t_flat_tree *ft)
{
	t_flat_tree	*left;
	t_flat_tree	*flat;

	flat = flat_tree_last(ft);
	if (flat->tree_node->empty == -1)
	{
		flat->prev->next = NULL;
		while (flat->prev)
			flat = flat->prev;
		flat->next->prev = NULL;
		left = flat->next;
		flat->tree_node->right = NULL;
		flat->tree_node->left = init_tree(left);
		return (flat->tree_node);
	}
	return (NULL);
}

static t_tree	*collect_pipe(t_flat_tree *ft)
{
	t_flat_tree	*left;
	t_flat_tree	*flat;
	t_flat_tree	*right;
	int			p;

	p = 0;
	flat = flat_tree_last(ft);
	while (flat->prev)
	{
		skip_parenths(flat, &p);
		if (p == 0 && flat->tree_node->type == PIPE)
		{
			right = flat->next;
			left = flat->prev;
			flat->next->prev = NULL;
			flat->prev->next = NULL;
			flat->next = NULL;
			flat->prev = NULL;
			flat->tree_node->right = init_tree(right);
			flat->tree_node->left = init_tree(left);
			return (flat->tree_node);
		}
		flat = flat->prev;
	}
	return (NULL);
}

static t_tree	*collect_operator(t_flat_tree *ft)
{
	t_flat_tree	*left;
	t_flat_tree	*flat;
	t_flat_tree	*right;
	int			p;

	p = 0;
	flat = flat_tree_last(ft);
	while (flat->prev)
	{
		skip_parenths(flat, &p);
		if (p == 0 && (flat->tree_node->type == OP_OR || \
		flat->tree_node->type == OP_AND))
		{
			1 && (right = flat->next, left = flat->prev);
			flat->next->prev = NULL;
			flat->prev->next = NULL;
			flat->next = NULL;
			flat->prev = NULL;
			flat->tree_node->right = init_tree(right);
			flat->tree_node->left = init_tree(left);
			return (flat->tree_node);
		}
		flat = flat->prev;
	}
	return (NULL);
}

t_tree	*init_tree(t_flat_tree *ft)
{
	t_tree	*tree_node;

	tree_node = collect_operator(ft);
	if (tree_node)
		return (tree_node);
	tree_node = collect_pipe(ft);
	if (tree_node)
		return (tree_node);
	tree_node = collect_parenths(ft);
	if (tree_node)
		return (tree_node);
	return (ft->tree_node);
}
