#include "../../include/execution.h"


char *get_env_path_parent(const char *path);
int recover_invalid_pwd(t_env **env, char *new_pwd);
int diagnose_cd_error(const char *path, int print_error)
{
    struct stat st;

    if (access(path, F_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: no such file or directory: %s\n", path);
        return (DIR_STATUS_NOT_EXI);
    }
    if (access(path, X_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: permission denied: %s\n", path);
        return (DIR_STATUS_NO_PER);
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
    int status;
    chdir(arg);
    status =  (diagnose_cd_error(new_old_pwd, 1));
    if (status == DIR_STATUS_NO_PER)
    {
        old_pwd = ft_strjoin(new_old_pwd, arg);
        free(arg);
        free(new_old_pwd);
       return (recover_invalid_pwd(env, old_pwd));
    }
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
       // parent = NULL;
        parent = temp;
    }
    else
        parent = old_pwd;
    set_env_var("OLDPWD", logic_pwd, env);
    set_env_var("PWD", parent, env);
    return (free(arg), free(old_pwd) ,0);
}


char *get_env_path_parent(const char *path)
{
     char *parent;
    int len = strlen(path);

    while (len > 1 && path[len - 1] == '/')
        len--;

    int i = len - 1;
    while (i >= 0 && path[i] != '/')
        i--;

    if (i < 0)
        return strdup("/");

    if (i == 0)
        return strdup("/");

    parent = malloc(i + 1);
    if (!parent)
        return NULL;
    strncpy(parent, path, i);
    parent[i] = '\0';
    return parent;
}

int	recover_invalid_pwd(t_env **env, char *new_pwd)
{
	char	*start;
	char	*cur;
	char	*next;
	int		steps;
	int		i;

	if (!new_pwd)
		return (1);
	steps = 0;
	i = -1;
	while (new_pwd[++i])
		if (new_pwd[i] == '.' && new_pwd[i + 1] == '.')
			steps++;
	start = strdup(get_env_value("PWD", *env));
	if (!start)
		return (free(new_pwd), 1);
	cur = start;
	while (steps-- > 0)
	{
		next = get_env_path_parent(cur);
		free(cur);
		if (!next)
			return (free(new_pwd), 1);
		cur = next;
	}
	if (access(cur, X_OK) == 0 && chdir(cur) == 0)
	{
		set_env_var("PWD", cur, env);
		free(cur);
		return (free(new_pwd), 0);
	}
	free(cur);
	free(new_pwd);
	return (1);
}

// int recover_invalid_pwd(t_env **env, char *new_pwd)
// {
//     char *fake_pwd;
//     int steps_back;
//     char *parent;
//     char *new_parent;
//     int i;
//     if (!new_pwd)
//         return 1;
//     steps_back = 0;
//     i = 0;
//     while (new_pwd[i])
//     {
//         if (new_pwd[i] == '.' && new_pwd[i + 1] != '\0' && new_pwd[i + 1] == '.')
//             steps_back++;
//         i++;
//     }
//     fake_pwd = get_env_value("PWD", *env);
//     fake_pwd = ft_strdup(fake_pwd);
//     if (!fake_pwd)
//     {
//         return 1;
//     }
//     parent = fake_pwd;
//     while (steps_back-- > 0)
//     {
//         new_parent = get_env_path_parent(parent);
//         if (!new_parent)
//         {
//             free(parent);
//             free(fake_pwd);
//              free(new_pwd);
//             return 1;
//         }
//         free(parent);
//         parent = NULL;
//         parent = new_parent;
//     }
//     if (access(parent, X_OK) == 0 && chdir(parent) == 0)
//     {
//         set_env_var("PWD", parent, env);
//      //   free(parent);
//        free(new_pwd);
//         return 0;
//     }
//     free(parent);
//     free(fake_pwd);
//      free(new_pwd);
//     return 1;
// }

/*

int	recover_invalid_pwd(t_env **env, char *new_pwd)
{
	char	*start;
	char	*cur;
	char	*next;
	int		steps;
	int		i;

	if (!new_pwd)
		return (1);
	steps = 0;
	i = -1;
	while (new_pwd[++i])
		if (new_pwd[i] == '.' && new_pwd[i + 1] == '.')
			steps++;
	start = strdup(get_env_value("PWD", *env));
	if (!start)
		return (free(new_pwd), 1);
	cur = start;
	while (steps-- > 0)
	{
		next = get_env_path_parent(cur);
		free(cur);
		if (!next)
			return (free(new_pwd), 1);
		cur = next;
	}
	if (access(cur, X_OK) == 0 && chdir(cur) == 0)
	{
		set_env_var("PWD", cur, env);
		free(cur);
		return (free(new_pwd), 0);
	}
	free(cur);
	free(new_pwd);
	return (1);
}


*/