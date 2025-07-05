/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 23:38:52 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/04 23:59:01 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

void	split_cleaner(t_node *list)
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

void	skip_spaces(char *str, int *i)
{
	while (*(str + *i) && ft_isspace(*(str + *i)))
		(*i)++;
}
