
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

int update_last_executed_command(t_env **env_list,char *key, char *last_command)
{
	t_env *env_tmp;
	char *tmp_last_command;
	 env_tmp = *env_list;
	 if (!last_command)
	 	return 1;
	while (env_tmp != NULL)
	{
		if (ft_strcmp(env_tmp->key, key) == 0)
		{
			free(env_tmp->value);
			env_tmp->value = ft_strdup(last_command);
			free (last_command);
			return 0;
		}
		env_tmp = env_tmp->next;
	}
	free (last_command);
	return 1;
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
		perror("error: ");
		free(key);
		free(value);
		return (R_FAIL);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->exported = 1;
	new_node->is_remove = 1;
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
	return (R_SUCCESS);
}

int set_pwd_and_oldpwd_if_not_found(t_env **env_list)
{
	t_env *temp_env;
	t_env *temp;
	int		is_pwd;

	temp_env = *env_list;
	temp = *env_list;
	is_pwd = 0;
	while (temp_env != NULL)
	{
		if (!ft_strcmp(temp_env->key, "PWD"))
			is_pwd = 1;
		if (is_pwd == 1)
			break;
		temp_env = temp_env->next;
	}
	if (is_pwd == 1)
			return 1;
	while (temp->next != NULL)
		temp = temp->next;
	char *pwd_value = getcwd(NULL,0);
	char *pwd_key= ft_strdup("PWD");

	 create_env_node(env_list, pwd_key, pwd_value);
	return 1;
}
int	env_generate(t_env **env_list, char **env)
{
	char	*key;
	char	*value;
	int		i;
	
	if (!env || !(*env))
	{
		return (init_env(env_list));
	}
	i = -1;
	while (env[++i])
	{
		if (env_extract_key(env[i], &key) == R_FAIL)
			return (perror("error: "), free_env_list(*env_list), R_FAIL);
		if (env_extract_value(env[i], &value) == R_FAIL)
			return (perror("error: "), free(key), free_env_list(*env_list), R_FAIL);
		if (create_env_node(env_list, key, value) == R_FAIL)
			return (perror("error: "), free_env_list(*env_list), R_FAIL);
	}
	key = ft_strdup("exit_status@gmail.com");
	if (!key)
		return (perror("error: "), free_env_list(*env_list), R_FAIL);
	value = ft_strdup("0");
	if (!value)
		return (perror("error: "), free(key), free_env_list(*env_list), R_FAIL);
	if (create_env_node(env_list, key, value) == R_FAIL)
		return (perror("error: "), free_env_list(*env_list), R_FAIL);
	set_pwd_and_oldpwd_if_not_found(env_list);
	return (R_SUCCESS);
}
