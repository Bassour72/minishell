/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_expand_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:52:17 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/02 14:30:41 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static void	init_token(t_expand_token *token, char *data, int join, int split)
{
	token->data = data;
	token->join = join;
	token->split = split;
}

int	append_expand_token(t_expand_token **tokens, \
	char *data, int split, int join, int is_var)
{
	t_expand_token	*new_token;
	t_expand_token	*tmp;

	new_token = malloc(sizeof(t_expand_token));
	if (!new_token)
		return (perror("error: "), free(data), R_FAIL);
	init_token(new_token, data, join, split);
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->is_var = is_var;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
		new_token->prev = tmp;
	}
	return (R_SUCCESS);
}
