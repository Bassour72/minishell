/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:21:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/03 20:18:05 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	generate_new_data_str_red(char **dst, char *str)
{
	int	size;
	int	i;
	int	quote;

	quote = -1;
	i = -1;
	size = 0;
	while (*(str + ++i))
	{
		if (*(str + i) == '\"' || *(str + i) == '\'')
		{
			if (quote == -1)
				quote = *(str + i);
			if (quote == *(str + i))
				continue;	
		}
			size++;
	}
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	quote = -1;
		i = 0;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
		{
			if (quote == -1)
				quote = *str;
			if (quote == *str)
			{
				str++;
				continue;
			}
		}
		*(*dst + i++) = *str;
		str++;
	}
	*(*dst + i) = '\0';
	print(*dst);
	return (R_SUCCESS);
}

static char	*delimiter_clear_dollar(char *str)
{
	int		len;
	char	*new_str;
	int		i;

	len = 0;
	i = -1;
	while (*(str + ++i))
	{
		if (*(str + i) == '$' && *(str + i + 1) && !ft_isspace(*(str + i + 1)) && (*(str + i + 1) == '\"' || *(str + i + 1) == '\''))
		{

			*(str + i) = DOUBLE_QUOTE;
		}
		else
			len++;
	}
	new_str = malloc(len + 1);
	if (!new_str)
		return (perror("error: "), NULL);
	int	j = 0;
	i = -1;
	while (*(str + ++i))
	{
		if (*(str + i) != DOUBLE_QUOTE)
			*(new_str + j++) = *(str + i);
	}
	*(new_str + j) = '\0';
	free(str);
	return (new_str);
}

static int	remove_non_printable_chars(t_red *reds)
{
	char	*new_data;

	while (reds)
	{
		if(reds->type == HER_DOC)
		{
			if ((reds->data = delimiter_clear_dollar(reds->data)) == NULL)
				return (R_FAIL);
		}
		if (reds->is_ambiguous == 0)
		{
			if (generate_new_data_str_red(&new_data, reds->data) == R_FAIL) //this may cause leaks
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
		free(splited_line);
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
