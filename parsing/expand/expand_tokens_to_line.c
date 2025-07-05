/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_to_line.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:20:27 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 16:36:29 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	calculate_new_line_len(t_expand_token *tokens)
{
	int	len;

	len = 0;
	while (tokens)
	{
		if (tokens->data)
			len += ft_strlen(tokens->data) + 1;
		tokens = tokens->next;
	}
	return (len);
}

static int	is_able_to_join(t_expand_token *token)
{
	char	quote;

	if (ft_strlen(token->data) == 2 && token->join  && ((token->next && token->next->data[0]) || \
	(!token->next)))
	{
		quote = token->data[0];
		if ((quote == '\'' || quote == '\"') && token->data[0] == token->data[1])
			return (0);
	}
	return (1);
}

int	expand_tokens_to_line(char **new_line, t_expand_token *tokens)
{
	int	len;

	len = calculate_new_line_len(tokens) + 1;
	*new_line = ft_calloc(len, 1);
	if (!*new_line)
		return (perror("error: "), R_FAIL);
	while (tokens)
	{
		if (tokens->data && is_able_to_join(tokens))
		{
			ft_strlcat(*new_line, tokens->data, len);
			if (tokens->next && !tokens->join)
				ft_strlcat(*new_line, " ", len);
		}
		tokens = tokens->next;
	}
	return (R_SUCCESS);
}
