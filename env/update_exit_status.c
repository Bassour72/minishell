/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:32:36 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 18:33:31 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env.h"

int	update_env_exit_status(t_env **env_list, int status)
{
	char	*exit_str;
	t_env	*env_tmp;

	exit_str = ft_itoa(status);
	if (!exit_str)
		return (perror("error: "), R_FAIL);
	env_tmp = *env_list;
	while (env_tmp)
	{
		if (ft_strcmp(env_tmp->key, EXIT_STATUS_KEY) == 0)
		{
			free(env_tmp->value);
			env_tmp->value = exit_str;
		}
		env_tmp = env_tmp->next;
	}
	return (R_SUCCESS);
}
