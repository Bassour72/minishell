/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote_expander.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:01:43 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 17:26:00 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	calculate_new_data_len(char *str, int _i)
{
	int	len;

	len = 0;
	if (_i == 0)
		len++;
	while (1)
	{
		if (str[len] == '\"')
		{
			len++;
			break ;
		}
		else if (str[len] == '$' && !ft_isspace(str[len + 1]) && \
		str[len + 1] != '\"')
		{
			break ;
		}
		len++;
	}
	return (len);
}

static int	extract_data(char *str, int *_i, char **data)
{
	char	*new_data;
	char	*tmp;
	int		len;

	len = calculate_new_data_len(str, *_i);
	(*_i) += len;
	new_data = ft_substr(str, 0, len);
	if (!new_data)
		return (R_FAIL);
	tmp = *data;
	*data = ft_strjoin(*data, new_data);
	(free(tmp), free(new_data));
	if (!*data)
		return (R_FAIL);
	len = ft_strlen(*data);
	if (len > 0 && (*data)[len - 1] == '\"')
		return (R_DONE);
	return (R_SUCCESS);
}

static int	is_done(char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len >= 2 && data[len - 1] == '\"')
		return (1);
	return (0);
}

int	double_quote_expander(char *str, int *i, t_expand_token **tokens, \
	t_env *env)
{
	int		_i;
	int		state;
	char	*data;
	int		join;

	data = NULL;
	_i = 0;
	while (1)
	{
		if (str[_i] == '$' && str[_i + 1] != '\"' && !ft_isspace(str[_i + 1]))
		{
			if (double_quote_variable_expander(str, &_i, &data, env) == R_FAIL)
				return (R_FAIL);
		}
		else
		{
			if (extract_data(str + _i, &_i, &data) == R_FAIL)
				return (R_FAIL);
			else if (is_done(data))
				break ;
		}
	}
	(*i) += _i;
	return (finish_and_append_double_quote_expand(str, _i, data, tokens));
}
