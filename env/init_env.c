#include "../include/parsing.h"

// static t_env	*create_env_node(char *key, char *value,
// 					   int exported, int is_remove)
// {
// 	t_env	*node;

// 	node = malloc(sizeof(t_env));
// 	if (node == NULL)
// 	{
// 		return (NULL);
// 	}
// 	node->key = ft_strdup(key);
// 	if (node->key == NULL)
// 	{
// 		free(node);
// 		return (NULL);
// 	}
// 	if (value != NULL)
// 	{
// 		node->value = ft_strdup(value);
// 		if (node->value == NULL)
// 		{
// 			free(node->key);
// 			free(node);
// 			return (NULL);
// 		}
// 	}
// 	else
// 	{
// 		node->value = NULL;
// 	}
// 	node->exported = exported;
// 	node->is_remove = is_remove;
// 	node->next = NULL;
// 	return (node);
// }

// static int	link_env_nodes(t_env **env_list)
// {
// 	const char	*path;
// 	t_env		*oldpwd;
// 	t_env		*pwd;
// 	t_env		*shlvl;
// 	t_env		*path_node;
// 	t_env		*exit_status;

// 	path = "/usr/local/bin:/usr/local/sbin:"
// 		    "/usr/bin:/usr/sbin:/bin:/sbin:.";
// 	oldpwd = create_env_node("OLDPWD", NULL, 1, 1);
// 	if (oldpwd == NULL)
// 	{
// 		return (1);
// 	}
// 	pwd = create_env_node("PWD",
// 			   "/home/ybassour/Desktop/minishell", 1, 1);
// 	if (pwd == NULL)
// 	{
// 		free(oldpwd->key);
// 		free(oldpwd);
// 		return (1);
// 	}
// 	shlvl = create_env_node("SHLVL", "0", 1, 1);
// 	if (shlvl == NULL)
// 	{
// 		free(pwd->key);
// 		free(pwd->value);
// 		free(pwd);
// 		free(oldpwd->key);
// 		free(oldpwd);
// 		return (1);
// 	}
// 	path_node = create_env_node("PATH", (char *)path, 0, 1);
// 	if (path_node == NULL)
// 	{
// 		free(shlvl->key);
// 		free(shlvl->value);
// 		free(shlvl);
// 		free(pwd->key);
// 		free(pwd->value);
// 		free(pwd);
// 		free(oldpwd->key);
// 		free(oldpwd);
// 		return (1);
// 	}
// 	exit_status = create_env_node("exit_status@gmail.com", "0", 0, 1);
// 	if (exit_status == NULL)
// 	{
// 		free(path_node->key);
// 		free(path_node->value);
// 		free(path_node);
// 		free(shlvl->key);
// 		free(shlvl->value);
// 		free(shlvl);
// 		free(pwd->key);
// 		free(pwd->value);
// 		free(pwd);
// 		free(oldpwd->key);
// 		free(oldpwd);
// 		return (1);
// 	}
// 	oldpwd->next      = pwd;
// 	pwd->next         = shlvl;
// 	shlvl->next       = path_node;
// 	path_node->next   = exit_status;
// 	*env_list        = oldpwd;
// 	return (0);
// }

// int	init_env(t_env **env_list)
// {
// 	if (env_list == NULL)
// 	{
// 		return (1);
// 	}
// 	return (link_env_nodes(env_list));
// }


t_env	*create_env_node(char *key, char *value, int exported, int is_remove)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->key = ft_strdup(key);
	if (node->key == NULL)
		return (free(node), NULL);
	if (value != NULL)
	{
		node->value = ft_strdup(value);
		if (node->value == NULL)
			return (free(node->key), free(node), NULL);
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
	return (1);
}

static int	env_create_defaults(t_env **env_list)
{
	const char	*path = "/usr/local/bin:/usr/local/sbin:"
						"/usr/bin:/usr/sbin:/bin:/sbin:.";
	t_env	*oldpwd = create_env_node("OLDPWD", NULL, 1, 1);
	t_env	*pwd = create_env_node("PWD", "/home/ybassour/Desktop/minishell", 1, 1);
	t_env	*shlvl = create_env_node("SHLVL", "0", 1, 1);
	t_env	*path_node = create_env_node("PATH", (char *)path, 0, 1);
	t_env	*exit_status = create_env_node("exit_status@gmail.com", "0", 0, 1);

	if (!oldpwd || !pwd || !shlvl || !path_node || !exit_status)
		return (env_list_cleanup(oldpwd, pwd, shlvl, path_node));
	oldpwd->next = pwd;
	pwd->next = shlvl;
	shlvl->next = path_node;
	path_node->next = exit_status;
	*env_list = oldpwd;
	return (0);
}

int	init_env(t_env **env_list)
{
	if (env_list == NULL)
		return (1);
	return (env_create_defaults(env_list));
}
