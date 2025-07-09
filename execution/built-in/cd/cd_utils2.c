/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:03:00 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/09 16:05:34 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

void	update_pwd(t_env **env)
{
	char	*newcwd;

	newcwd = getcwd(NULL, 0);
	if (newcwd)
	{
		set_env_var("PWD", newcwd, env);
		free(newcwd);
	}
}

char	*get_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	cd_dotdots_only(t_env **env, char *arg)
{
	char	*logical_pwd;
	char	*old_pwd;
	char	*cwd;

	old_pwd = NULL;
	logical_pwd = get_env_value("physical_PWD", *env);
	old_pwd = ft_strdup(logical_pwd);
	if (chdir(arg) == 0)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			cwd = build_logical_path(old_pwd, arg);
		set_env_var("OLDPWD", old_pwd, env);
		set_env_var("PWD", cwd, env);
		return (free_all(arg, old_pwd, cwd));
	}
	if (handle_cd_failure(env, arg, old_pwd) == 0)
		return (free_all(arg, old_pwd, NULL), 1);
	perror("cd: Permission denied");
	return (free_all(arg, old_pwd, NULL));
}
