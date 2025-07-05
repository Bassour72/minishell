/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:07:32 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 10:08:30 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

t_tree	*new_tree_node(t_type type)
{
	t_tree	*tree_node;

	tree_node = malloc(sizeof(t_tree));
	if (!tree_node)
		return (perror("error: "), NULL);
	tree_node->data = NULL;
	tree_node->redirections = NULL;
	tree_node->type = type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->empty = 0;
	return (tree_node);
}

t_tree	*init_tree(t_flat_tree *ft)
{
	t_flat_tree		*flat;
	t_flat_tree		*right;
	t_flat_tree		*left;
	int p = 0;

	flat =  flat_tree_last(ft);
	while (flat->prev)
	{
		if (flat->tree_node->empty == -1)
			p++;
		else if (flat->tree_node->empty == 1)
			p--;
		else if (p == 0 && (flat->tree_node->type == OP_OR || flat->tree_node->type == OP_AND))
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

	p = 0;
	flat =  flat_tree_last(ft);
	while (flat->prev)
	{
		if (flat->tree_node->empty == -1)
			p++;
		else if (flat->tree_node->empty == 1)
			p--;
			
		else if (p == 0 && flat->tree_node->type == PIPE)
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

	flat = flat_tree_last(ft);
	if (flat->tree_node->empty == -1)//  )
	{
		flat->prev->next = NULL;//  ) = NULL
		// flat->prev = NULL; // maybe this is unessessiry
		while (flat->prev)
		{
			flat = flat->prev;
			// printf("he\n");
		}

		flat->next->prev = NULL;
		// flat->prev = NULL; // maybe this is unessessiry
		left = flat->next;
		flat->tree_node->left = init_tree(left);
		return (flat->tree_node);
	}
	return (ft->tree_node);
}
