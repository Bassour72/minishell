/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:21:56 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 21:32:22 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

int	print_environment(t_env *env_list)
{
	t_env	*tmp_env;

	if (!env_list)
		return (1);
	tmp_env = env_list;
	while (tmp_env)
	{
		if (tmp_env->key && tmp_env->value)
		{
			if (strcmp(tmp_env->key, "exit_status@gmail.com") != 0)
			{
				if (ft_strcmp("physical_PWD", tmp_env->key) != 0)
					printf("%s=%s\n", tmp_env->key, tmp_env->value);
			}
		}
		tmp_env = tmp_env->next;
	}
	return (0);
}

int	env_environment(t_tree *root, t_env *env_list)
{
	if (root->data[1])
	{
		display_error(" : No such file or directory\n", root->data[1]);
		return (127);
	}
	return (print_environment(env_list));
}
