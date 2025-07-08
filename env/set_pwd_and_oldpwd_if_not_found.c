/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pwd_and_oldpwd_if_not_found.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:48:47 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 16:42:55 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env.h"

static int	check_if_key_already_exist(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (1);
		env_list = env_list->next;
	}
	return (0);
}

static int	set_pwd_and_oldpwd_if_not_found(t_env **env_list, char *key)
{
	t_env	*temp;
	char	*pwd_value;
	char	*pwd_key;

	temp = *env_list;
	if (check_if_key_already_exist(*env_list, key) == 1)
		return (R_SUCCESS);
	while (temp->next)
		temp = temp->next;
	pwd_value = getcwd(NULL, 0);
	pwd_key = ft_strdup(key);
	if (!pwd_key)
		return (perror("error: "), free_env_list(*env_list), R_FAIL);
	if (create_env_node(env_list, pwd_key, pwd_value) == R_FAIL)
		return (free_env_list(*env_list), R_FAIL);
	return (R_SUCCESS);
}

int	set_pwd_and_physical_pwd_(t_env **env_list)
{
	if (set_pwd_and_oldpwd_if_not_found(env_list, "physical_PWD") == R_FAIL)
		return (R_FAIL);
	if (set_pwd_and_oldpwd_if_not_found(env_list, "PWD") == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}
