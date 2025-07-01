/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_variable_expander.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:22:03 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/01 11:09:58 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	is_valid_key(char *key)
{
	int	i;

	if (!key[0])
		return (0);
	i = -1;
	while (key[++i] && key[i] != '=')
	{
		if (is_valid_key_char(key[i], i) == 0)
			return (0);
	}
	return (1);
}

static int	is_befor_var_valid(t_expand_token *tokens)
{
	while (tokens->next)
		tokens = tokens->next;
	if (ft_strcmp(tokens->data, "=") == 0)
		tokens = tokens->prev;
	if (tokens->join == 1 && tokens->split == 1 && is_valid_key(tokens->data))
		return (1);
	return (0);
}

int	is_export(char *line)
{
	if (ft_memcmp("export ", line, 7) == 0 && is_valid_key_char(line[7], 0))
		return (1);
	return (0);
}

int	normal_variable_expander(char *str, int *i, t_expand_token **tokens, \
	t_env *env)
{
	char	*data;
	int		join;
	int		split;

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
			char *tmp;
			int len = ft_strlen(data) + 3;
			tmp = ft_calloc(len, 1);
			ft_strlcat(tmp, "\x0F", len);
			ft_strlcat(tmp, data, len);
			ft_strlcat(tmp, "\x0F", len);
			free(data);
			data = tmp;
		}
	}
	join = 1;
	if (!str[*i] || str[*i] == ' ')
		join = 0;
	if (append_expand_token(tokens, data, split, join) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
