/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:32:55 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 18:35:32 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env.h"

char	*env_get_value(t_env *list, char *key)
{
	if (!key || !list)
		return (NULL);
	while (list)
	{
		if (ft_memcmp(list->key, key, ft_strlen(key) + 1) == 0)
		{
			if (ft_strcmp(key, "physical_PWD") == 0)
				return (NULL);
			return (list->value);
		}
		list = list->next;
	}
	return (NULL);
}

void	free_env_list(t_env *list)
{
	t_env	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
