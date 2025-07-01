/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list_to_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:45:50 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/01 09:08:19 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	expand_list_to_array(char ***new_args, t_node *splited_line)
{
	t_node	*tmp;
	int		i;

	*new_args = malloc(sizeof(char *) * (get_list_size(splited_line) + 1));
	if (!*new_args)
		return (perror("error: "), free(splited_line), R_FAIL);
	
	tmp = splited_line;
	i = 0;
	while (tmp)
	{
		(*new_args)[i++] = tmp->data;
		tmp = tmp->next;
	}
	(*new_args)[i] = NULL;
	while (splited_line)
	{
		tmp = splited_line;
		splited_line = splited_line->next;
		free(tmp);
	}
	return (R_SUCCESS);
}
