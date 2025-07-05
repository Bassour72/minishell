/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:50:54 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:58:53 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	is_special(char *c)
{
	return (*c == '(' || *c == ')' || *c == '<' || *c == '>' \
	|| *c == '|' || *c == '&');
}

static int	quotes(char *str, char **data, t_node **list, int *i)
{
	if (!m_quotes(str, data, i))
		return (split_cleaner(*list), R_FAIL);
	if (*data && (ft_isspace(str[*i]) || is_special(str + *i) \
	|| !str[*i]) && !append_node(list, data))
		return (split_cleaner(*list), R_FAIL);
	return (R_SUCCESS);
}

static int	specials(char *str, char **data, t_node **list, int *i)
{
	if (*data && !append_node(list, data))
		return (split_cleaner(*list), R_FAIL);
	if (!m_operators(list, str, i))
		return (split_cleaner(*list), R_FAIL);
	if (!m_parenth(list, str, i))
		return (split_cleaner(*list), R_FAIL);
	if (!m_redirections(list, str, i))
		return (split_cleaner(*list), R_FAIL);
	return (R_SUCCESS);
}

static int	normal(char *str, char **data, t_node **list, int *i)
{
	if (!m_normal(str, data, i))
		return (split_cleaner(*list), R_FAIL);
	if (*data && (is_special(str + *i) || ft_isspace(str[*i]) \
	|| !str[*i]) && !append_node(list, data))
		return (split_cleaner(*list), R_FAIL);
	return (R_SUCCESS);
}

int	split(t_node **list, char *str)
{
	char	*data;
	int		i;

	data = NULL;
	i = 0;
	while (*(str + i) != '\0')
	{
		skip_spaces(str, &i);
		if (*(str + i) == '\"' || *(str + i) == '\'')
		{
			if (quotes(str, &data, list, &i) == R_FAIL)
				return (R_FAIL);
		}
		else if (is_special(str + i))
		{
			if (specials(str, &data, list, &i) == R_FAIL)
				return (R_FAIL);
		}
		else
			if (normal(str, &data, list, &i) == R_FAIL)
				return (R_FAIL);
		skip_spaces(str, &i);
	}
	return (R_SUCCESS);
}
