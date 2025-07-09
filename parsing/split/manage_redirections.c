/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:38:52 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 13:59:41 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	append(t_node **list, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (perror("error"), R_FAIL);
	data[0] = '>';
	data[1] = '>';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

static int	trunk(t_node **list, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (perror("error"), R_FAIL);
	data[0] = '>';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

static int	herdoc(t_node **list, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (perror("error"), R_FAIL);
	data[0] = '<';
	data[1] = '<';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

static int	input(t_node **list, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (perror("error"), R_FAIL);
	data[0] = '<';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

int	m_redirections(t_node **list, char *str, int *i)
{
	int		state;
	char	*data;

	state = 1;
	data = NULL;
	if (*(str + *i) == '<' && *(str + *i + 1) == '<')
		state = herdoc(list, i);
	else if (*(str + *i) == '<')
		state = input(list, i);
	else if (*(str + *i) == '>' && *(str + *i + 1) == '>')
		state = append(list, i);
	else if (*(str + *i) == '>')
		state = trunk(list, i);
	return (state);
}
