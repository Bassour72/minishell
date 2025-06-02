#include "../../include/execution.h"
#include <sys/stat.h>

#define DIR_STATUS_NO_PER 11
#define DIR_STATUS_NOT_EXI 12
#define DIR_STATUS_IS_RM 13

char *get_env_path_parent(const char *path)
{
    int len = strlen(path);

    // Skip trailing slashes
    while (len > 0 && path[len - 1] == '/')
        len--;
    // Find the last '/' before the basename
    while (len > 0 && path[len - 1] != '/')
        len--;

    // If there's no parent (e.g., "/"), return "/"
    if (len == 0)
        return strdup("/");

    char *parent = malloc(len + 1);
    if (!parent)
        return NULL;

    strncpy(parent, path, len);//todo change strncpy ft_strncpy
    parent[len] = '\0';

    return parent;
}


static int check_argument(t_tree *root)
{
    if (root && root->data && root->data[0] && root->data[1] && root->data[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    return 0;
}

static int diagnose_cd_error(const char *path, int print_error)
{
    struct stat st;
    
    if (access(path, F_OK) != 0)
    {
         printf("here for here go her=f_ok \n");
        if (print_error)
            fprintf(stderr, "cd: no such file or directory: %s\n", path);
        return DIR_STATUS_NOT_EXI;
    }
    if (stat(path, &st) != 0)
    {
         printf("here for here go her=stat \n");
        perror("cd: stat");
        return 1;
    }
    if (!S_ISDIR(st.st_mode))
    {
         printf("here for here go her=s_isdir \n");
         if (print_error)
            fprintf(stderr, "cd: not a directory: %s\n", path);
        printf("THE DIR IS REMOVE \n\n\n\n");
        return DIR_STATUS_NOT_EXI;;
    }
    if (access(path, X_OK) != 0)
    {
         printf("here for here go her=********************x_ok \n");
         if (print_error)
            fprintf(stderr, "cd: permission denied: %s\n", path);
        return DIR_STATUS_NO_PER;
    }
    return 0; // fallback
}

char *get_env_value(char *key, t_env *env)
{
    while (env)
    {
        if (!strcmp(env->key, key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

void set_env_var(char *key, char *value, t_env **env)
{
    t_env *tmp;
    t_env *new;

    tmp = *env;
    while (tmp)
    {
        if (!strcmp(tmp->key, key))
        {
            free(tmp->value);
            tmp->value = strdup(value);
            return;
        }
        tmp = tmp->next;
    }
    new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->key = strdup(key);
    new->value = strdup(value);
    new->exported = 1;
    new->next = *env;
    *env = new;
}

static char *expand_home(char *arg, t_env *env)
{
    char *home;
    char *res;

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

static char *resolve_path(char *arg, t_env *env)
{
    if (!arg || arg[0] == '~')
        return (expand_home(arg, env));
    if (!strcmp(arg, "-"))
    {
        arg = get_env_value("OLDPWD", env);
        if (!arg)
            return (NULL);
        printf("%s\n", arg);
        return (strdup(arg));
    }
    return (strdup(arg));
}

int cd_change_working_directory(t_tree *root, t_env **env)
{
    char *old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        char *env_pwd = get_env_value("PWD", *env);
        if (!env_pwd)
        {
            fprintf(stderr, "cd: error retrieving current directory\n");
            return 1;
        }
        old_pwd = strdup(env_pwd);
    }

    if (check_argument(root))
        return free(old_pwd), 1;

    char *target_dir = NULL;
    int dir_status;

    if (!root->data[1]) // cd with no argument
    {
        target_dir = get_env_value("HOME", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return free(old_pwd), 1;
        }
        target_dir = strdup(target_dir);
    }
    else if (!strcmp(root->data[1], "-")) // cd -
    {
        target_dir = get_env_value("OLDPWD", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return free(old_pwd), 1;
        }
        target_dir = strdup(target_dir);
        printf("%s\n", target_dir);
    }
    else
    {
        dir_status = diagnose_cd_error(old_pwd, 0);

        // Handle inaccessible/removed current dir + `cd ..`
        if (!strcmp(root->data[1], "..") && (dir_status == DIR_STATUS_NO_PER || dir_status == DIR_STATUS_NOT_EXI))
        {
            char *fake_pwd = get_env_value("PWD", *env);
            if (!fake_pwd)
            {
                fprintf(stderr, "cd: cannot determine current directory\n");
                return free(old_pwd), 1;
            }
            char *parent = get_env_path_parent(fake_pwd);
            if (!parent)
                return free(old_pwd), 1;

            if (chdir(parent) == 0)
            {
                set_env_var("OLDPWD", old_pwd, env);
                set_env_var("PWD", parent, env);
                free(old_pwd);
                free(parent);
                return 0;
            }
            perror("cd");
            free(old_pwd);
            free(parent);
            return 1;
        }

        // Normal path resolution
        target_dir = resolve_path(root->data[1], *env);
        if (!target_dir)
        {
            free(old_pwd);
            return 1;
        }
    }

    if (chdir(target_dir) != 0)
    {
        perror("cd");
        free(old_pwd);
        free(target_dir);
        return 1;
    }

    // Update OLDPWD and PWD
    set_env_var("OLDPWD", old_pwd, env);
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
        set_env_var("PWD", new_pwd, env);
        free(new_pwd);
    }
    
    else
    {
        set_env_var("PWD", target_dir, env); // fallback
    }
    free(old_pwd);
    free(target_dir);
    return 0;
}

/*
int cd_change_working_directory(t_tree *root, t_env **env)
{
    char *old_pwd = getcwd(NULL, 0);
    char *logical_pwd = get_env_value("PWD", *env);
    char *target_dir = NULL;
    int result;

    if (check_argument(root))
        return 1;

    // Handle no args => cd to HOME
    if (!root->data[1])
    {
        target_dir = get_env_value("HOME", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            free(old_pwd);
            return 1;
        }
        target_dir = strdup(target_dir);
    }
    // Handle cd -
    else if (!strcmp(root->data[1], "-"))
    {
        target_dir = get_env_value("OLDPWD", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(old_pwd);
            return 1;
        }
        target_dir = strdup(target_dir);
        printf("%s\n", target_dir);
    }
    // Handle cd ..
    else if (!strcmp(root->data[1], ".."))
    {
        // If getcwd fails (e.g., directory deleted or inaccessible), fallback to logical PWD
        char *base = old_pwd ? old_pwd : (logical_pwd ? strdup(logical_pwd) : NULL);
        if (!base)
            return fprintf(stderr, "cd: error retrieving current directory\n"), 1;

        target_dir = get_env_path_parent(base);
        free(base);

        if (!target_dir || chdir(target_dir) != 0)
        {
            perror("cd");
            free(target_dir);
            free(old_pwd);
            return 1;
        }

        set_env_var("OLDPWD", logical_pwd, env);
        set_env_var("PWD", target_dir, env);
        free(target_dir);
        free(old_pwd);
        return 0;
    }
    // All other arguments
    else
    {
        target_dir = resolve_path(root->data[1], *env);
        if (!target_dir)
        {
            free(old_pwd);
            return 1;
        }
    }

    // Normal chdir
    result = chdir(target_dir);
    if (result != 0)
    {
        perror("cd");
        free(target_dir);
        free(old_pwd);
        return 1;
    }

    // Update environment
    if (old_pwd)
    {
        set_env_var("OLDPWD", old_pwd, env);
        free(old_pwd);
    }
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd)
    {
        set_env_var("PWD", new_pwd, env);
        free(new_pwd);
    }
    else
    {
        set_env_var("PWD", target_dir, env); // fallback to logical
    }

    free(target_dir);
    return 0;
}

*/