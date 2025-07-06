/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_variable_expander.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:22:03 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 22:14:42 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int	is_export(char *line)
{
	if (ft_memcmp("export ", line, 7) == 0)
		return (1);
	return (0);
}

static int	is_value(char *str, int i)
{
	return (*(str + i - 1) == '=');
}

static void	update_value(char *value)
{
	if (!value)
		return ;
	while (*value)
	{
		if (*value == '\"')
			*value = DOUBLE_QUOTE;
		else if (*value == '\'')
			*value = SINGLE_QUOTE;
		value++;
	}
}

int	normal_variable_expander(char *str, int *i, t_expand_token **tokens, \
	t_env *env)
{
	char	*data;
	int		join;
	char	*tmp;
	int		len;

	if (!is_valid_key_char(str[*i + 1], 0) && str[*i + 1] != '?')
		return ((*i) += 2, R_CONTINUE);
	if (extract_var_value(str + *i, i, &data, env) == R_FAIL)
		return (R_FAIL);
	
	if (is_export(str) && is_befor_var_valid(*tokens))
	{
		len = ft_strlen(data) + 3;
		tmp = ft_calloc(len, 1);
		ft_strlcat(tmp, "\"", len);
		ft_strlcat(tmp, data, len);
		ft_strlcat(tmp, "\"", len);
		free(data);
		data = tmp;
	}
	join = 1;
	if (!str[*i] || ft_isspace(str[*i]))
		join = 0;
	if (append_expand_token(tokens, data, join) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
