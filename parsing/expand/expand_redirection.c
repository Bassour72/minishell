/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:21:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 21:37:56 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/parsing.h"

static int	is_empty(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

static int	set_new_data_or_ambiguous(t_red *red_node, char **new_arr)
{
	int	i;

	if (new_arr && new_arr[0] && new_arr[1])
	{
		free_2d_arr(new_arr);
		red_node->is_ambiguous = 1;
	}
	else
	{
		free(red_node->data);
		red_node->data = new_arr[0];
		free(new_arr);
	}
	return (R_SUCCESS);
}

static int	tokenize_then_split(t_red *reds, t_env *env, t_node **splited_line)
{
	t_expand_token	*tokens;
	char			*new_line;
	
	tokens = NULL;
	if (tokenize(reds->data, &tokens, env) == R_FAIL)
		return (R_FAIL);
	if (expand_tokens_to_line(&new_line, tokens) == R_FAIL)
		return (free_expand_tokens_list(tokens), R_FAIL);
	free_expand_tokens_list(tokens);
	if (expand_split2(splited_line, new_line) == R_FAIL)
		return (free(new_line), R_FAIL);
	free(new_line);
	return (R_SUCCESS);
}

int	expand_redir(t_red *reds, t_env *env)
{
	t_node			*splited_line;
	char			**new_arr;

	while (reds)
	{
		if (reds->type != HER_DOC)
		{
			splited_line = NULL;
			if (tokenize_then_split(reds, env, &splited_line) == R_FAIL)
				return (R_FAIL);
			if (expand_list_to_array(&new_arr, splited_line) == R_FAIL)
				return (R_FAIL);
			if(wildcard(&new_arr) == R_FAIL)
				return (R_FAIL);
			if(remove_non_printable_characters(&new_arr) == R_FAIL)
				return (R_FAIL);
			recover_quotes(new_arr);
			if (set_new_data_or_ambiguous(reds, new_arr) == R_FAIL)
				return (R_FAIL);
		}
		reds = reds->next;
	}
	return (R_SUCCESS);
}
