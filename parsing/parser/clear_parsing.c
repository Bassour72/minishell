/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:13:41 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 00:33:09 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

void	free_tree_node(t_tree *node)
{
	int		i;
	t_red	*red;
	t_red	*tmp;

	if (node->data)
	{
		i = -1;
		while (node->data[++i])
			free(node->data[i]);
		free(node->data);
	}
	if (node->redirections)
	{
		red = node->redirections;
		while (red)
		{
			tmp = red;
			red = red->next;
			free(tmp->data);
			free(tmp);
		}
	}
	free(node);
}

void	free_flat_tree_list(t_flat_tree *flat_list)
{
	t_flat_tree	*tmp;

	while (flat_list)
	{
		tmp = flat_list;
		flat_list = flat_list->next;
		free_tree_node(tmp->tree_node);
		free(tmp);
	}
}

void	free_flat_tree(t_flat_tree *flat_tree)
{
	t_flat_tree	*tmp;

	while (flat_tree)
	{
		tmp = flat_tree;
		flat_tree = flat_tree->next;
		free_tree_node(tmp->tree_node);
		free(tmp);
	}
}

void	free_tree(t_tree *root)
{
	if (root->right)
		free_tree(root->right);
	if (root->left)
		free_tree(root->left);
	free_tree_node(root);
}
