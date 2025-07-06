#include "../../../include/execution.h"

char	*get_env_value_v2(char *key, t_env *env)
{
	while (env)
	{
		if (!strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	pwd_print_working_directory(t_tree *root, t_env **env_list)
{
	char	*pathname;
	int		fd; 

	fd = STDOUT_FILENO;
	if (!root || !env_list)
		return (1);
	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		pathname = get_env_value_v2("physical_PWD", *env_list);
		if (!pathname)
		{
			perror("pwd: getcwd");
			return (1);
		}
		write(fd, pathname, ft_strlen(pathname));
		write(fd, "\n", 1);
		return (0);
	}
	write(fd, pathname, ft_strlen(pathname));
	write(fd, "\n", 1);
	free(pathname);
	return (0);
}
