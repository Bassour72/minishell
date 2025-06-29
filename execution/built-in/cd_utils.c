#include "../../include/execution.h"

int diagnose_cd_error(const char *path, int print_error)
{
    struct stat st;

    if (access(path, X_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: permission denied: %s\n", path);
        return (DIR_STATUS_NO_PER);
    }
    if (access(path, F_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: no such file or directory: %s\n", path);
        return (DIR_STATUS_NOT_EXI);
    }
    if (stat(path, &st) != 0)
    {
        if (print_error)
            perror("cd: stat");
        return (DIR_STATUS_NOT_EXI);
    }
    if (!S_ISDIR(st.st_mode))
    {
        if (print_error)
            fprintf(stderr, "cd: not a directory: %s\n", path);
        return (DIR_STATUS_NOT_EXI);
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
		return ;
	create_env_var(key, value, env);
}


// void set_env_var(char *key, char *value, t_env **env)
// {
//     t_env *tmp;
// 	char *new_value;
// 	t_env *new;

// 	tmp = *env;
//     while (tmp)
//     {
//         if (!ft_strcmp(tmp->key, key))
//         {
//             new_value = ft_strdup(value);
//             if (new_value)
//             {
//                 free(tmp->value);
//                 tmp->value = new_value;
//             }
//             return ;
//         }
//         tmp = tmp->next;
//     }
//     new = malloc(sizeof(t_env));
//     if (!new)
//         return ;
//     new->key = ft_strdup(key);
//     new->value = ft_strdup(value);
//     new->exported = 1;
//     new->is_remove = 1;
//     new->next = *env;
//     *env = new;
// }

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

int apply_cd_with_double_dots(t_tree *root, t_env **env, char *arg)
{
    char *logic_pwd;
    char *parent;
    char *temp;
    char *old_pwd;
    char *new_old_pwd = getcwd(NULL, 0);

    if (chdir(arg) != 0)
       return (diagnose_cd_error(new_old_pwd, 1), free(new_old_pwd), 1);
    free(new_old_pwd);
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
         perror("cd: error retrieving current directory: getcwd:");
    
    logic_pwd = get_env_value("PWD", *env);
    if (!logic_pwd)
        logic_pwd = old_pwd;
    parent = get_env_value("PWD", *env);
    if (!parent)
        return (free(old_pwd), 1);
    if (old_pwd == 0)
    {
        if (ft_strlen(arg) > 2)
            temp = ft_strjoin(parent, arg);
        else
            temp = ft_strjoin(parent, "/..");
        parent = temp;
    }
    else
        parent = old_pwd;
    set_env_var("OLDPWD", logic_pwd, env);
    set_env_var("PWD", parent, env);
    return (free(arg), free(old_pwd) ,0);
}
