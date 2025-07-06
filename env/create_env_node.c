/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:42:40 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 23:05:01 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"


int	create_env_node(t_env **list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ((perror("error: "), free(key), free(value)), R_FAIL);
	new_node->key = key;
	new_node->value = value;
	new_node->exported = 1;
	new_node->is_remove = 1;
	if (!ft_strcmp(key, "_") || !ft_strcmp(key, "exit_status@gmail.com") || \
	!ft_strcmp(key, "physical_PWD")) 
		new_node->exported = 0;
	new_node->next = NULL;

	if (!*list)
		*list = new_node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (R_SUCCESS);
}
