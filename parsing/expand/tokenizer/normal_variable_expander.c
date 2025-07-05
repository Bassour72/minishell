/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_variable_expander.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:22:03 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 16:39:23 by massrayb         ###   ########.fr       */
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
		if ((quote == '\'' || quote == '\"') && token->data[0] == token->data[1])
			return (1);
	}
	return (0);
}

static int	is_befor_var_valid(t_expand_token *tokens)
{
	int	is_value;

	is_value = 0;
	while (tokens->next)
		tokens = tokens->next;
	while (tokens->prev)
	{
		if (tokens->join == 0)
			break ;
		if (tokens->data[ft_strlen(tokens->data) - 1] == '=')
			is_value = 1;
		tokens = tokens->prev;
		if(is_value)
			break ;
	}
	if (is_value == 0)
		return (0);
	while (tokens->prev)
	{
		if (tokens->join == 0)
			break;
		if (!is_valid_key(tokens->data) || is_empty_key(tokens))
			return (0);
		tokens = tokens->prev;
	}
	return (1);
}

int	is_export(char *line)
{
	if (ft_memcmp("export ", line, 7) == 0)
		return (1);
	return (0);
}

static int is_value(char *str, int i)
{
	return (*(str + i - 1) == '=');
}

int	normal_variable_expander(char *str, int *i, t_expand_token **tokens, \
	t_env *env)
{
	char	*data;
	int		join;
	int		split;
	char	*tmp;

	data = NULL;
	if (!is_valid_key_char(str[*i + 1], 0) && str[*i + 1] != '?')
		return ((*i) += 2, R_CONTINUE);
	if (extract_var_value(str + *i, i, &data, env) == R_FAIL)
		return (R_FAIL);
	split = 1;
	if (is_export(str))
	{
			if (is_befor_var_valid(*tokens))
			{
				int len = ft_strlen(data) + 3;
				tmp = ft_calloc(len, 1);
				ft_strlcat(tmp, "\"", len);
				ft_strlcat(tmp, data, len);
				ft_strlcat(tmp, "\"", len);
				free(data);
				data = tmp;
			}
		
	}
	join = 1;
	if (!str[*i] || ft_isspace(str[*i]))
		join = 0;
	if (append_expand_token(tokens, data, split, join, 1) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
