/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:21:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 15:00:01 by ybassour         ###   ########.fr       */
/*   Updated: 2025/07/05 00:20:04 by massrayb         ###   ########.fr       */
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

void	set_new_data_or_ambiguous(t_red *red_node, t_node *splited_line)
{
	int	i;

	if (splited_line && splited_line->data && splited_line->next)
	{
		free_list(splited_line);
		red_node->is_ambiguous = 1;
	}
	else
	{
		i = 0;
		while (*(splited_line->data + i))
		{
			if (*(splited_line->data + i) == DOUBLE_QUOTE)
				*(splited_line->data + i) = '\"';
			else if (*(splited_line->data + i) == SINGLE_QUOTE)
				*(splited_line->data + i) = '\'';
			i++;
		}
		free(red_node->data);
		red_node->data = splited_line->data;
		free(splited_line);
	}
}

int	expand_redir(t_red *reds, t_env *env)
{
	t_expand_token	*tokens;
	t_node			*splited_line;
	char			*new_line;

	//printf("+=-=-==-=-=-==\n");
	while (reds)
	{
		if (reds->type != HER_DOC)
		{
			splited_line = NULL;
			tokens = NULL;
			if (tokenize(reds->data, &tokens, env) == R_FAIL)
				return (R_FAIL);
			if (expand_tokens_to_line(&new_line, tokens) == R_FAIL)
				return (free_expand_tokens_list(tokens), R_FAIL);
			//printf("{%s}[%d]\n", new_line, ft_strlen(new_line));
			free_expand_tokens_list(tokens);
			if (expand_split2(&splited_line, new_line) == R_FAIL)
				return (free(new_line), R_FAIL);
			free(new_line);
			set_new_data_or_ambiguous(reds, splited_line);
		}
		reds = reds->next;
	}
	return (R_SUCCESS);
}
