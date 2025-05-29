#include "../../include/execution.h"



char	*get_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (!strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	set_env_var(char *key, char *value, t_env **env)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (!strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = strdup(key);
	new->value = strdup(value);
	new->exported = 1;
	new->next = *env;
	*env = new;
}

static char	*expand_home(char *arg, t_env *env)
{
	char	*home;
	char	*res;

	home = get_env_value("HOME", env);
	if (!home)
		return (NULL);
	res = malloc(strlen(home) + strlen(arg));
	if (!res)
		return (NULL);
	strcpy(res, home);
	strcat(res, arg + 1);
	return (res);
}

static char	*resolve_path(char *arg, t_env *env)
{
	if (!arg || arg[0] == '~')
		return (expand_home(arg, env));
	if (!strcmp(arg, "-"))
	{
		arg = get_env_value("OLDPWD", env);
		if (!arg)
			return (NULL);
		printf("=====================%s\n", arg); //todo for debugg 
		return (strdup(arg));
	}
	return (strdup(arg));
}

int	cd_change_working_directory(t_tree *root, t_env **env)
{
	char	*old;
	char	*new;
	char	*dir;
	int		res;

	old = getcwd(NULL, 0);
	if (!old)
	{
        if (root->data[0] && ft_strcmp(root->data[1], "-") == 0)
            return 1;
        else if (root->data[0] && root->data[1])
        {
            chdir(root->data[1]);
            return (1);
        }
        
        dir = get_env_value("HOME", *env);
		if (!dir || chdir(dir) != 0)
		{
			perror("cd");
			return (1);
		}
		set_env_var("PWD", dir, env);
		return (0);
	}
	dir = resolve_path(root->data[1], *env);
	if (!dir)
	{
		free(old);
		return (1);
	}
	res = chdir(dir);
	if (!res)
	{
		set_env_var("OLDPWD", old, env);
		new = getcwd(NULL, 0);
		if (new)
		{
			set_env_var("PWD", new, env);
			free(new);
		}
	}
	else
		perror("cd");
	free(old);
	free(dir);
	return (res != 0);
}



