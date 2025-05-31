#include "../../include/execution.h"
#include <sys/stat.h>


static int check_argument(t_tree *root)
{
    if (root && root->data && root->data[0] && root->data[1] && root->data[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    return 0;
}

static int diagnose_cd_error(const char *path)
{
    struct stat st;

    if (access(path, F_OK) != 0)
    {
        fprintf(stderr, "cd: no such file or directory: %s\n", path);
        return 1;
    }
    if (stat(path, &st) != 0)
    {
        perror("cd: stat");
        return 1;
    }
    if (!S_ISDIR(st.st_mode))
    {
        fprintf(stderr, "cd: not a directory: %s\n", path);
        return 1;
    }
    if (access(path, X_OK) != 0)
    {
        fprintf(stderr, "cd: permission denied: %s\n", path);
        return 1;
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
    char *target_dir = NULL;
    int result = 0;

    if (check_argument(root))
        return 1;

    if (!root->data[1]) // cd
    {
        target_dir = get_env_value("HOME", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            free(old_pwd);
            return 1;
        }
    }
    else if (!strcmp(root->data[1], "-")) // cd -
    {
        target_dir = get_env_value("OLDPWD", *env);
        if (!target_dir)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(old_pwd);
            return 1;
        }
        printf("%s\n", target_dir);
    }
    else
    {
        printf("here for here go her=********************e \n");
        if (diagnose_cd_error(old_pwd))
        {
            if (strcmp(root->data[1] , "..") == 0)
                printf("here for here go here \n");
                return 1;
        }
        target_dir = resolve_path(root->data[1], *env);
        if (!target_dir)
        {
            free(old_pwd);
            return 1;
        }
    }

    // Try to change directory
    result = chdir(target_dir);
    if (result != 0)
    {
        perror("cd");
        free(old_pwd);
        if (root->data[1] && strcmp(root->data[1], "-") != 0)
            free(target_dir);
        return 1;
    }
    // Update OLDPWD and PWD
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
        set_env_var("PWD", target_dir, env);
    }

    if (root->data[1] && strcmp(root->data[1], "-") != 0)
        free(target_dir);
    return 0;
}
