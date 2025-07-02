/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote_expander.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:00:04 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/02 10:39:51 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	calculate_data_len(char *str)
{
	int	i;

	i = 1;
	while (str[i] != '\'')
		i++;
	return (i + 1);
}

static void	convert_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\"')
			*str = DOUBLE_QUOTE;
		str++;
	}
}

int	single_quote_expander(char *str, int *i, t_expand_token **tokens)
{
	char	*data;
	int		len;
	int		_i;
	int		join;

	len = calculate_data_len(str);
	data = ft_substr(str, 0, len);
	if (!data)
		return (perror("error: "), free_expand_tokens_list(*tokens), R_FAIL);
	convert_quotes(data);
	// data[0] = SINGLE_QUOTE;
	// data[len - 1] = SINGLE_QUOTE;
	join = 1;
	if (str[len] && ft_isspace(str[len]))
		join = 0;
	if (append_expand_token(tokens, data, 0, join, 0) == R_FAIL)
		return (free_expand_tokens_list(*tokens), R_FAIL);
	*i += len;
	return (R_SUCCESS);
}
