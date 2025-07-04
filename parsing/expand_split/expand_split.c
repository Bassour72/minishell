/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:07:12 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 00:08:40 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static void	cleaner(t_node *list)
{
	t_node	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

static void	_skip_spaces(char *str, int *i)
{
	while (*(str + *i) && ft_isspace(*(str + *i)))
		(*i)++;
}

static int	expand_quotes(char *str, char **data, t_node **list, int *i)
{
	if (!m_quotes(str, data, i))
		return (cleaner(*list), R_FAIL);
	if (*data && (ft_isspace(str[*i]) || is_special(str + *i) \
	|| !str[*i]) && !append_node(list, data))
		return (cleaner(*list), R_FAIL);
	return (R_SUCCESS);
}

static int	normal(char *str, char **data, t_node **list, int *i)
{
	if (!m_expand_normal(str, data, i))
		return (cleaner(*list), R_FAIL);
	if (*data && (ft_isspace(str[*i]) || !str[*i]) && !append_node(list, data))
		return (cleaner(*list), R_FAIL);
	return (R_SUCCESS);
}

int	expand_split2(t_node **list, char *str)
{
	char	*data;
	int		i;

	data = NULL;
	i = 0;
	while (*(str + i) != '\0')
	{
		_skip_spaces(str, &i);
		if (*(str + i) == '\"' || *(str + i) == '\'')
		{
			if (expand_quotes(str, &data, list, &i) == R_FAIL)
				return (R_FAIL);
		}
		else
		{
			if (normal(str, &data, list, &i) == R_FAIL)
				return (R_FAIL);
		}
		_skip_spaces(str, &i);
	}
	return (R_SUCCESS);
}
