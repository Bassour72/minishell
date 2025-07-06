/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_befor_var_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:39:42 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 18:45:10 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	is_valid_key(char *key)
{
	int	i;

	if (!key[0])
		return (0);
	i = -1;
	while (key[++i])
	{
		if (key[i] == '=' && key[i + 1] == '\0')
			return (1);
		if (is_valid_key_char(key[i], i) == 0)
			return (0);
	}
	return (1);
}

static int	is_empty_key(t_expand_token *token)
{
	char	quote;

	if (ft_strlen(token->data) == 2 && (token->next && token->next->data[0]))
	{
		quote = token->data[0];
		if ((quote == '\'' || quote == '\"') && \
		token->data[0] == token->data[1])
			return (1);
	}
	return (0);
}

static void	update_is_value(t_expand_token *tokens, int *is_value)
{
	if (tokens->data[ft_strlen(tokens->data) - 1] == '=')
		*is_value = 1;
}

int	is_befor_var_valid(t_expand_token *tokens)
{
	int	is_value;

	is_value = 0;
	while (tokens->next)
		tokens = tokens->next;
	while (tokens->prev)
	{
		if (tokens->join == 0)
			break ;
		update_is_value(tokens, &is_value);
		tokens = tokens->prev;
		if (is_value)
			break ;
	}
	if (is_value == 0)
		return (0);
	while (is_value && tokens->prev)
	{
		if (tokens->join == 0)
			break ;
		if (!is_valid_key(tokens->data) || is_empty_key(tokens))
			return (0);
		tokens = tokens->prev;
	}
	return (1);
}
