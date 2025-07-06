/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pwd_and_oldpwd_if_not_found.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:48:47 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 23:36:56 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

static int	set_pwd_and_oldpwd_if_not_found(t_env **env_list, char *key)
{
	t_env	*temp_env;
	t_env	*temp;
	int		is_pwd;

	temp_env = *env_list;
	temp = *env_list;
	is_pwd = 0;
	while (temp_env != NULL)
	{
		if (!ft_strcmp(temp_env->key, key))
			is_pwd = 1;
		if (is_pwd == 1)
			break ;
		temp_env = temp_env->next;
	}
	if (is_pwd == 1)
			return (0);
	while (temp->next != NULL)
		temp = temp->next;
	char *pwd_value = getcwd(NULL,0);
	char *pwd_key= ft_strdup(key);
	return (!create_env_node(env_list, pwd_key, pwd_value));
}


int  set_pwd_and_physical_pwd_(t_env **env_list)
{
	int status;

	status = set_pwd_and_oldpwd_if_not_found(env_list, "physical_PWD");
	status = set_pwd_and_oldpwd_if_not_found(env_list, "PWD");
	return (status);
}