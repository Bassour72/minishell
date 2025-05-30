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


int	is_exist_directory(t_tree *root)
{
	if (!root || !root->data)
		return 1;
	else if (root->data[0] && root->data[1])
	{
		if (access(root->data[1], F_OK) != 0)
		{
			printf("the directory not exist[%d]  \n", access(root->data[1], F_OK));
			return 1;
		}
	}
	return 0;
}

 int has_permission(t_tree *root, t_env **env, char *current)
{
	is_exist_directory(root);
	if (!root || !root->data)
	{
		printf("root =NULL || root->data == NULL \n");
		return (1);
	}
	if (root->data[0] && !root->data[1])
	{
		printf("root =NULL || root->data == NULL \n");
		return (1);
	}
	if (access(current, X_OK) == 0)
	{
		printf("the dirctory does not has permission for change \n");
		return (1);
	}
	return 0;
}
int	cd_change_working_directory(t_tree *root, t_env **env)
{
	char	*old;
	char	*new;
	char	*dir;
	int		res;

	old = getcwd(NULL, 0);
	if (has_permission(root, env, old))
		return 1;
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



