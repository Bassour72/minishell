#include "../../include/execution.h"


char *get_env_path_parent(const char *path);
int recover_invalid_pwd(t_env **env, char *new_pwd);

int diagnose_cd_error(const char *path, int print_error)
{
    struct stat st;

    if (stat(path, &st) != 0)
    {
        if (print_error || print_error  == -1)
            fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        return (1);
    }
    if (!S_ISDIR(st.st_mode))
    {
        if (print_error || print_error  == -1)
            fprintf(stderr, "cd: %s: Not a directory\n", path);
        return (2);
    }
    if (chdir(path) != 0)
    {
        if (print_error || print_error  == -1)
            fprintf(stderr, "cd: %s:\n", strerror(errno));
        return (3);
    }
    return (0);
}

char * get_arg_cd(const char *arg)
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
	update_env_var("physical_PWD", value, *env);
	create_env_var(key, value, env);
}


void update_pwd(t_env **env)
{
    char *newcwd;

    newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        set_env_var("PWD", newcwd, env);
        free(newcwd);
    }
}

 char *get_env_value(char *key, t_env *env)
{
    while (env)
    {
        if (!ft_strcmp(env->key, key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}




/**************************************************************************************************** */

static int	count_double_dots(const char *path)
{
	int i = 0;
	int count = 0;

	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] == '.')
			count++;
		i++;
	}
	return (count);
}

static char	*build_logical_path(char *base, char *suffix)
{
   perror("cd: error retrieving current directory: getcwd: cannot access parent directories:\n");
	if (ft_strlen(suffix) > 2)
		return (ft_strjoin(base, suffix));
	return (ft_strjoin(base, "/.."));
}

static int	free_all(char *a, char *b, char *c)
{
	if (a)
		free(a);
	if (b)
		free(b);
	if (c)
		free(c);
	return (0);
}

char	*get_env_path_parent(const char *path)
{
	char	*parent;
	int		len;
	int		i;

	len = ft_strlen(path);
	while (len > 1 && path[len - 1] == '/')
		len--;
	i = len - 1;
	while (i >= 0 && path[i] != '/')
		i--;
	if (i <= 0)
		return (ft_strdup("/"));
	parent = malloc(i + 1);
	if (!parent)
		return (NULL);
	ft_strlcpy(parent, path, i + 1);
	return (parent);
}

static int	handle_cd_failure(t_env **env, char *arg, char *logical_pwd)
{
	char	*fallback;
	char	*next;
	int		steps;
	int		i;

	if (!logical_pwd)
		return (1);
	fallback = ft_strdup(logical_pwd);
	steps = count_double_dots(arg);
	i = 0;
	while (i++ < steps)
	{
		next = get_env_path_parent(fallback);
		if (!next)
			return (free(fallback), 1);
		free(fallback);
		fallback = next;
	}
	if (access(fallback, X_OK) == 0 && chdir(fallback) == 0)
	{
		set_env_var("OLDPWD", logical_pwd, env);
		set_env_var("PWD", fallback, env);
		return (free(fallback), 0);
	}
	return (free(fallback), 1);
}

int	apply_cd_with_double_dots(t_tree *root, t_env **env, char *arg)
{
	char	*logical_pwd;
	char	*old_pwd;
	char	*cwd;

	(void)root;
	old_pwd = NULL;
	logical_pwd = get_env_value("physical_PWD", *env);
    old_pwd =  ft_strdup(logical_pwd);
	if (chdir(arg) == 0)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			cwd = build_logical_path(old_pwd, arg);
		set_env_var("OLDPWD", old_pwd, env);
		set_env_var("PWD", cwd, env);
		return (free_all(arg, old_pwd, cwd));
	}
	if (handle_cd_failure(env, arg, old_pwd) == 0)
		return (free_all(arg, old_pwd, NULL));
	perror("cd: Permission denied");
	return (free_all(arg, old_pwd, NULL));
}


