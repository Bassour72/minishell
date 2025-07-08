/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:44:31 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 13:23:47 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env.h"

static int	env_extract_key(char *env, char **key)
{
	char	*end;
	int		len;

	end = ft_strchr(env, '=');
	len = end - env;
	*key = malloc(len + 1);
	if (!key)
		return (perror("error: "), R_FAIL);
	ft_strlcpy(*key, env, len + 1);
	return (R_SUCCESS);
}

static int	env_extract_value(char *env, char **value)
{
	char	*start;

	start = ft_strchr(env, '=') + 1;
	if (*start == '\0')
		return (*value = NULL, R_SUCCESS);
	*value = ft_strdup(start);
	if (!*value)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

static int	add_exit_status(t_env **env_list)
{
	char	*key;
	char	*value;

	key = ft_strdup(EXIT_STATUS_KEY);
	if (!key)
		return (perror("error: "), free_env_list(*env_list), R_FAIL);
	value = ft_strdup("0");
	if (!value)
		return (perror("error: "), free(key), free_env_list(*env_list), R_FAIL);
	if (create_env_node(env_list, key, value) == R_FAIL)
		return (free_env_list(*env_list), R_FAIL);
	return (R_SUCCESS);
}

int	env_generate(t_env **env_list, char **env)
{
	char	*key;
	char	*value;
	int		i;

	if (!env || !(*env))
	{
		if (init_env(env_list) == R_FAIL)
			return (R_FAIL);
		return (set_pwd_and_physical_pwd_(env_list));
	}
	i = -1;
	while (env[++i])
	{
		if (env_extract_key(env[i], &key) == R_FAIL)
			return (free_env_list(*env_list), R_FAIL);
		if (env_extract_value(env[i], &value) == R_FAIL)
			return (free(key), free_env_list(*env_list), R_FAIL);
		if (create_env_node(env_list, key, value) == R_FAIL)
			return (free_env_list(*env_list), R_FAIL);
	}
	if (add_exit_status(env_list) == R_FAIL)
		return (R_FAIL);
	return (set_pwd_and_physical_pwd_(env_list));
}
