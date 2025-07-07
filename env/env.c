
#include "../include/parsing.h"

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

int	env_generate(t_env **env_list, char **env)
{
	char	*key;
	char	*value;
	int		i;
	int		status;

	if (!env || !(*env))
	{
		status = init_env(env_list);
		if (status != 0)
			return (R_FAIL);
		status =  set_pwd_and_physical_pwd_(env_list);
		return (status);
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
	set_pwd_and_physical_pwd_(env_list);
	return (R_SUCCESS);
}
