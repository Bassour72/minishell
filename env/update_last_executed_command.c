/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_last_executed_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:38:44 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 22:41:10 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	update_last_executed_command(t_env **env_list, char *key, \
char *last_command)
{
	t_env	*env_tmp;
	char	*tmp_last_command;

	env_tmp = *env_list;
	if (!last_command)
		return (1);
	while (env_tmp != NULL)
	{
		if (ft_strcmp(env_tmp->key, key) == 0)
		{
			free(env_tmp->value);
			tmp_last_command = ft_strdup(last_command);
			env_tmp->value = tmp_last_command;
			free (last_command);
			return (0);
		}
		env_tmp = env_tmp->next;
	}
	free (last_command);
	return (1);
}
