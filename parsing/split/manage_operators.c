/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_operators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 09:57:26 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:36:11 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	_or(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (0);
	data[0] = '|';
	data[1] = '|';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

static int	_pipe(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (0);
	data[0] = '|';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

static int	_and(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (0);
	data[0] = '&';
	data[1] = '&';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

int	m_operators(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	state = 1;
	data = NULL;
	if (*(str + *i) == '|' && *(str + *i + 1) == '|')
		state = _or(list, str, i);
	else if (*(str + *i) == '|')
		state = _pipe(list, str, i);
	else if (*(str + *i) == '&')
		state = _and(list, str, i);
	return (state);
}
