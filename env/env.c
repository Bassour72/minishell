
#include "../include/parsing.h"

void print_env(t_env *env) //note to debug
{
	while (env)
	{
		printf("[%s]->[%s]\n", env->key, env->value);
		env = env->next;
	}
}


static int	env_extract_key(char *env, char **key)
{
	char *end; 
	int len;

	end = ft_strchr(env, '=');
	len  = end - env;
	*key = malloc(len + 1);
	if (!key)
		return (0);
	ft_strlcpy(*key, env, len + 1);
	return (1);
}

static int env_extract_value(char *env, char **value)
{
	char *start; 
	int len;

	start = ft_strchr(env, '=') + 1;
	if (*start == '\0')
		return (*value = NULL, 1);
	*value = ft_strdup(start);
	if (!*value)
		return (0);
	return (1);
}

static int	create_env_node(t_env **list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		free_env_list(*list);
		return (0);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;

	if (!*list)
		*list = new_node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (1);
}

int	env_generate(t_env **env_list, char **env)
{
	char	*key;
	char	*value;
	int		i;
	
	if (!env)
		return (-1);
	i = -1;
	while (env[++i])
	{
		if (!env_extract_key(env[i], &key) ||
			!env_extract_value(env[i], &value) || 
			!create_env_node(env_list, key, value))
		{
			return (0);
		}
	}
	return (1);
}


