/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_herdoc_delimiter.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:15:06 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 00:35:23 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	check_quote(int *quote, char c)
{
	int	state;

	if (c == '\'' || c == '\"')
	{
		if (*quote == 0)
		{
			*quote = c;
			return (R_CONTINUE);
		}
		else if (*quote == c)
		{
			*quote = 0;
			return (R_CONTINUE);
		}
	}
	return (R_SUCCESS);
}

static int	calculate_eof_size(char *str)
{
	int	size;
	int	quote;
	int	i;

	quote = 0;
	size = 0;
	i = -1;
	while (*(str + ++i))
	{
		if (check_quote(&quote, *(str + i)) == R_CONTINUE)
			continue ;
		size++;
	}
	return (size);
}

static int	remove_quotes_from_delimiter(char **dst, char *str)
{
	int	size;
	int	i;
	int	quote;

	size = calculate_eof_size(str);
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	quote = 0;
	i = 0;
	while (*str)
	{
		if (check_quote(&quote, *str) == R_SUCCESS)
			*(*dst + i++) = *str;
		str++;
	}
	*(*dst + i) = '\0';
	return (R_SUCCESS);
}

int	expand_herdoc_delimiter(t_red *reds, t_env *env)
{
	char	*new_data;

	while (reds)
	{
		if (reds->type == HER_DOC)
		{
			reds->data = delimiter_clear_dollar(reds->data);
			if (reds->data == NULL)
				return (R_FAIL);
			if (remove_quotes_from_delimiter(&new_data, reds->data) == R_FAIL)
				return (R_FAIL);
			free(reds->data);
			reds->data = new_data;
		}
		reds = reds->next;
	}
	return (R_SUCCESS);
}
