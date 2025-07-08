/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:14:25 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 13:14:44 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	is_red(t_token *token)
{
	if (token->type == T_FILE_NAME || \
		token->type == RED_INPUT || \
		token->type == HER_DOC || \
		token->type == RED_APPEND || \
		token->type == RED_TRUNK)
	{
		return (1);
	}
	return (0);
}

int	new_red(t_tree *tree_node, t_type type, char *data)
{
	t_red	*new_red;
	t_red	*last_red;

	new_red = malloc(sizeof(t_red));
	if (!new_red)
		return (free_tree_node(tree_node), R_FAIL);
	new_red->data = ft_strdup(data);
	if (!new_red->data)
	{
		free(new_red);
		free_tree_node(tree_node);
		return (R_FAIL);
	}
	1 && (new_red->type = type, new_red->is_ambiguous = 0);
	new_red->next = NULL;
	if (!tree_node->redirections)
		tree_node->redirections = new_red;
	else
	{
		last_red = tree_node->redirections;
		while (last_red->next)
			last_red = last_red->next;
		last_red->next = new_red;
	}
	return (1);
}

static void	skip_what_inside_parenths(t_token *token, int *i)
{
	int	p;

	p = 0;
	while (token[*i].data)
	{
		if (token[*i].type == PAREN_OPEN)
			p++;
		else if (token[*i].type == PAREN_CLOSE)
			p--;
		if (p == 0)
			break ;
		(*i) += 1;
	}
}

int	parenths_redirections(t_tree *tree_node, t_token *token)
{
	int		i;

	i = 0;
	skip_what_inside_parenths(token, &i);
	i++;
	while (token[i].data)
	{
		if ((token[i].type == RED_INPUT || \
			token[i].type == HER_DOC || \
			token[i].type == RED_APPEND || \
			token[i].type == RED_TRUNK) && \
			token[i].is_listed == 0)
		{
			(token + i)->is_listed = 1;
			if (new_red(tree_node, (token + i)->type, token[i + 1].data) == 0)
				return (R_FAIL);
		}
		else
			break ;
		i += 2;
	}
	return (R_SUCCESS);
}
