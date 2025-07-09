/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:04:38 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/09 11:48:05 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

int	diagnose_cd_error(const char *path, int print_error)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		if (print_error || print_error == -1)
			perror(strerror(errno));
		return (1);
	}
	if (!S_ISDIR(st.st_mode))
	{
		if (print_error || print_error == -1)
			perror("cd: ");
		return (2);
	}
	if (chdir(path) != 0)
	{
		if (print_error || print_error == -1)
			perror(strerror(errno));
		return (3);
	}
	return (0);
}

char	*get_arg_cd(const char *arg)
{
	if (!arg)
		return (NULL);
	return (ft_strdup(arg));
}

static int	update_env_var(char *key, char *value, t_env *env)
{
	char	*new_value;

	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			new_value = ft_strdup(value);
			if (new_value)
			{
				free(env->value);
				env->value = new_value;
			}
			return (1);
		}
		env = env->next;
	}
	return (0);
}

static void	create_env_var(char *key, char *value, t_env **env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->exported = 1;
	new->is_remove = 1;
	new->next = *env;
	*env = new;
}

void	set_env_var(char *key, char *value, t_env **env)
{
	if (update_env_var(key, value, *env))
	{
		if (!ft_strcmp("PWD", key))
			update_env_var("physical_PWD", value, *env);
		return ;
	}
	if (!ft_strcmp("PWD", key))
		update_env_var("physical_PWD", value, *env);
	create_env_var(key, value, env);
}
