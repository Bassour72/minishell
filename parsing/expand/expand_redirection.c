/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:21:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 22:29:13 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	generate_new_data_str(char **dst, char *str)
{
	int	size;
	int	i;

	i = -1;
	size = 0;
	while (*(str + ++i))
	{
		if (*(str + i) != DOUBLE_QUOTE && *(str + i) != SINGLE_QUOTE)
			size++;
	}
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	i = 0;
	while (*str)
	{
		if (*str != DOUBLE_QUOTE && *str != SINGLE_QUOTE)
			*(*dst + i++) = *str;
		str++;
	}
	*(*dst + i) = '\0';
	return (R_SUCCESS);
}

static int	remove_non_printable_chars(t_red *reds)
{
	char	*new_data;

	while (reds)
	{
		if (reds->type != HER_DOC && reds->is_ambiguous == 0)
		{
			if (generate_new_data_str(&new_data, reds->data) == R_FAIL)
				return (R_FAIL);
			free(reds->data);
			reds->data = new_data;
		}
		reds = reds->next;
	}
	return (R_SUCCESS);
}

void	set_new_data_or_ambiguous(t_red *red_node, t_expand_node *expanded_node)
{
	if (expanded_node && (expanded_node->data && !expanded_node->next))
	{
		free(red_node->data);
		red_node->data = expanded_node->data;
	}
	else
	{
		free_expand_list_nodes(expanded_node);
		red_node->is_ambiguous = 1;
	}
}

int	expand_redir(t_red *reds, t_env *env)
{
	t_expand_token	*tokens;
	t_expand_node	*expanded_node;
	t_red			*tmp;

	tmp = reds;
	while (reds)
	{
		if (reds->type != HER_DOC)
		{
			expanded_node = NULL;
			tokens = NULL;
			if (tokenize(reds->data, &tokens, env) == R_FAIL)
				return (R_FAIL);
			if (split_tokens_into_nodes(&expanded_node, tokens) == R_FAIL)
				return (free_expand_tokens_list(tokens), R_FAIL);
			free_expand_tokens_list(tokens);
			set_new_data_or_ambiguous(reds, expanded_node);
		}
		reds = reds->next;
	}
	if (tmp && remove_non_printable_chars(tmp) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
