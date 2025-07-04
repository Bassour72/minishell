/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:21:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 15:00:01 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	generate_new_data_str_red(char **dst, char *str)
{
	int	size;
	int	i;

	i = -1;
	size = 0;
	while (*(str + ++i))
	{
		if (*(str + i) != '\"' && *(str + i) != '\'')
			size++;
	}
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	i = 0;
	while (*str)
	{
		if (*str != '\"' && *str != '\'')
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
			if (generate_new_data_str_red(&new_data, reds->data) == R_FAIL)
				return (R_FAIL);
			free(reds->data);
			reds->data = new_data;
		}
		reds = reds->next;
	}
	return (R_SUCCESS);
}

void	set_new_data_or_ambiguous(t_red *red_node, t_node *splited_line)
{
	if (splited_line && (splited_line->data && !splited_line->next))
	{
		free(red_node->data);
		red_node->data = splited_line->data;
	}
	else
	{
		free_list(splited_line);
		red_node->is_ambiguous = 1;
	}
}

int	expand_redir(t_red *reds, t_env *env)
{
	t_expand_token	*tokens;
	t_node			*splited_line;
	t_red			*tmp;
	char 			*new_line;
	tmp = reds;
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
			free_expand_tokens_list(tokens);
			if (expand_split2(&splited_line, new_line) == R_FAIL)
				return (free(new_line), R_FAIL);
			free(new_line);
			
			set_new_data_or_ambiguous(reds, splited_line);
		}
		reds = reds->next;
	}
	if (tmp && remove_non_printable_chars(tmp) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
