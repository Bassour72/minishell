/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_parenth.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:36:35 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:37:05 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	open_p(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (0);
	data[0] = '(';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

static int	close_p(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (0);
	data[0] = ')';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

int	m_parenth(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	state = 1;
	data = NULL;
	if (*(str + *i) == '(')
		return (open_p(list, str, i));
	else if (*(str + *i) == ')')
		return (close_p(list, str, i));
	return (1);
}
