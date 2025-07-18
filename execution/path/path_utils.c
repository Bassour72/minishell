/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:55:33 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/06 23:58:05 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

char	*extract_path_variable(t_env **env_list)
{
	t_env	*env;

	env = *env_list;
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

char	*check_valid_command_path(char *command)
{
	if (!command)
		return (NULL);
	if (access(command, X_OK) == 0)
	{
		return (ft_strdup(command));
	}
	return (NULL);
}
