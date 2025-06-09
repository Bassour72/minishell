
#include "../include/parsing.h"

void print_env(t_env *env) //note to debug
{
	while (env)
	{
		// if (env->exported)
			printf("[%s]->[%s]\n", env->key, env->value);
		env = env->next;
	}
}

int update_last_executed_command(t_env **env_list, char *last_command)
{
	t_env *env_tmp;
	char *tmp_last_command;
	 env_tmp = *env_list;
	//  printf("update_last_executed_command(t_env **env_list, char *last_command)(%s)\n", last_command);
	while (env_tmp != NULL)
	{
		if (ft_strcmp(env_tmp->key, "_") == 0)
		{
			free(env_tmp->value);
			env_tmp->value = ft_strdup(last_command);
			return 0;
		}
		env_tmp = env_tmp->next;
	}
	return 1;
}

void print_debugg(char **env)
{
	while (*env != NULL)
	{
		printf("===[%s]\n", *env);
		env++;
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
	new_node->exported = 1;
	if (!ft_strcmp(key, "_") || !ft_strcmp(key, "exit_status@gmail.com"))
		new_node->exported = 0;
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
int	init_env(t_env **env_list)
{
	t_env *tmp;
	char const path[] = "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.";
	tmp = malloc(sizeof(t_env));
	tmp->key = ft_strdup("OLDPWD");
	tmp->value = NULL;
	tmp->exported = 1;
	tmp->next = NULL;
	/******************************** */
	tmp->next = malloc(sizeof(t_env));
	tmp->next->key = ft_strdup("PWD");
	tmp->next->value = ft_strdup("/home/ybassour/Desktop/minishell");
	tmp->next->exported = 1;
	tmp->next->next = NULL;
	/********************************************* */
	tmp->next->next = malloc(sizeof(t_env));
	tmp->next->next->key = ft_strdup("SHLVL");
	tmp->next->next->value = ft_strdup("0");;
	tmp->next->next->exported = 1;
	tmp->next->next->next = NULL;
	/*********************************/
	tmp->next->next->next = malloc(sizeof(t_env));
	tmp->next->next->next->key = ft_strdup("PATH");
	tmp->next->next->next->value = ft_strdup(path);;
	tmp->next->next->next->exported = 0;
	tmp->next->next->next->next = NULL;
	*env_list = tmp;
	return 1;
}
int	env_generate(t_env **env_list, char **env)
{
	char	*key;
	char	*value;
	int		i;
	//print_debugg(env);
	if (!env || !(*env))
	{
		init_env(env_list);
		return (-1);
	}
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
	create_env_node(env_list, "exit_status@gmail.com", "0");
	return (1);
}

// int	exit_code_status(int exit_code)
// {
// 	static int exi
// }
