/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_and_append_double_quote_expand.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:08:41 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 17:26:19 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static void	conver_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\'')
			*str = SINGLE_QUOTE;
		str++;
	}
}

int	finish_and_append_double_quote_expand(char *str, int _i, \
char *data, t_expand_token **tokens)
{
	int	join;

	join = 1;
	if (ft_isspace(str[_i]) || !str[_i])
		join = 0;
	conver_quotes(data);
	if (append_expand_token(tokens, data, join) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
