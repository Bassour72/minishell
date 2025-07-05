/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flat_tree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:34:21 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:24:55 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

t_flat_tree	*flat_tree_last(t_flat_tree *flat_tree)
{
	while (flat_tree->next)
		flat_tree = flat_tree->next;
	return (flat_tree);
}

int	skip_listed_tokens(t_token *token, int *i)
{
	if (token[*i].is_listed == 1)
	{
		(*i) += 2;
		return (1);
	}
	return (0);
}
