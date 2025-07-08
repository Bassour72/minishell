/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:32:34 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 16:42:24 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/env.h"

static t_env	*_create_env_node(char *key, char *value, int exported, \
int is_remove)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (perror("error: "), NULL);
	(void)key;
	node->key = ft_strdup(key);
	if (node->key == NULL)
		return (perror("error: "), free(node), NULL);
	if (value)
	{
		node->value = ft_strdup(value);
		if (node->value == NULL)
			return (perror("error: "), free(node->key), free(node), NULL);
	}
	else
		node->value = NULL;
	node->exported = exported;
	node->is_remove = is_remove;
	node->next = NULL;
	return (node);
}

static void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

static int	env_list_cleanup(t_env *a, t_env *b, t_env *c, t_env *d)
{
	free_env_node(d);
	free_env_node(c);
	free_env_node(b);
	free_env_node(a);
	return (R_FAIL);
}

int	init_env(t_env **env_list)
{
	t_env	*oldpwd;
	t_env	*pwd;
	t_env	*shlvl;
	t_env	*path_node;
	t_env	*exit_status;

	oldpwd = _create_env_node("OLDPWD", NULL, 1, 1);
	pwd = _create_env_node("PWD", "/home/ybassour/Desktop/minishell", 1, 1);
	shlvl = _create_env_node("SHLVL", "0", 1, 1);
	path_node = _create_env_node("PATH", "/usr/local/bin:/usr/local/sbin:" \
	"/usr/bin:/usr/sbin:/bin:/sbin:.", 0, 1);
	exit_status = _create_env_node("exit_status@gmail.com", "0", 0, 1);
	if (!oldpwd || !pwd || !shlvl || !path_node || !exit_status)
		return (env_list_cleanup(oldpwd, pwd, shlvl, path_node));
	oldpwd->next = pwd;
	pwd->next = shlvl;
	shlvl->next = path_node;
	path_node->next = exit_status;
	*env_list = oldpwd;
	return (R_SUCCESS);
}
