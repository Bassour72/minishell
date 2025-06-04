#include "../../include/execution.h"
#include <sys/stat.h>
#include <errno.h>


#include "../../include/execution.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define DIR_STATUS_NO_PER 11
#define DIR_STATUS_NOT_EXI 12


char *get_env_value(char *key, t_env *env)
{
    while (env)
    {
        if (!strcmp(env->key, key))
            return env->value;
        env = env->next;
    }
    return NULL;
}

void set_env_var(char *key, char *value, t_env **env)
{
    t_env *tmp = *env;
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
    t_env *new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->key      = strdup(key);
    new->value    = strdup(value);
    new->exported = 1;
    new->next     = *env;
    *env          = new;
}

char *get_env_path_parent(const char *path)
{
    int len = strlen(path);

    /* Strip trailing slashes, but leave "/" if path == "/" */
    while (len > 1 && path[len - 1] == '/')
        len--;

    /* Find last '/' in path[0..len-1] */
    int i = len - 1;
    while (i >= 0 && path[i] != '/')
        i--;

    /* No '/' found → parent is "/" */
    if (i < 0)
        return strdup("/");

    /* If that '/' is at index 0, parent is "/" */
    if (i == 0)
        return strdup("/");

    /* Otherwise, copy path[0..i-1] into parent */
    char *parent = malloc(i + 1);
    if (!parent)
        return NULL;
    strncpy(parent, path, i);
    parent[i] = '\0';
    return parent;
}

static char *expand_home(char *arg, t_env *env)
{
    char *home = get_env_value("HOME", env);
    if (!home)
        return NULL;

    size_t needed = strlen(home) + strlen(arg);
    char *res = malloc(needed + 1);
    if (!res)
        return NULL;

    strcpy(res, home);
    strcat(res, arg + 1);
    return res;
}

static char *resolve_path(char *arg, t_env *env)
{
    if (!arg)
        return NULL;

    if (arg[0] == '~')
        return expand_home(arg, env);

    if (!strcmp(arg, "-"))
    {
        char *oldpwd = get_env_value("OLDPWD", env);
        if (!oldpwd)
            return NULL;
        printf("%s\n", oldpwd);
        return strdup(oldpwd);
    }

    return strdup(arg);
}

static int diagnose_cd_error(const char *path, int print_error)
{
    struct stat st;

    if (access(path, F_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: no such file or directory: %s\n", path);
        return DIR_STATUS_NOT_EXI;
    }

    if (stat(path, &st) != 0)
    {
        if (print_error)
            perror("cd: stat");
        return DIR_STATUS_NOT_EXI;
    }

    if (!S_ISDIR(st.st_mode))
    {
        if (print_error)
            fprintf(stderr, "cd: not a directory: %s\n", path);
        return DIR_STATUS_NOT_EXI;
    }

    if (access(path, X_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: permission denied: %s\n", path);
        return DIR_STATUS_NO_PER;
    }

    return 0;
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

int cd_change_working_directory(t_tree *root, t_env **env)
{
    /* 1) Save old_pwd = getcwd(); if getcwd() fails, fall back to env “PWD” */
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

    /* 2) Check for too many arguments */
    if (check_argument(root))
    {
        free(old_pwd);
        return 1;
    }

    char *arg       = (root->data[1] ? root->data[1] : NULL);
    char *candidate = NULL;
    int status;

    /* ----- Case A: no argument → cd $HOME ----- */
    if (!arg)
    {
        char *home = get_env_value("HOME", *env);
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            free(old_pwd);
            return 1;
        }
        candidate = strdup(home);
        if (!candidate)
        {
            free(old_pwd);
            return 1;
        }

        status = diagnose_cd_error(candidate, 1);
        if (status != 0)
        {
            free(candidate);
            free(old_pwd);
            return 1;
        }
        if (chdir(candidate) != 0)
        {
            perror("cd");
            free(candidate);
            free(old_pwd);
            return 1;
        }

        set_env_var("OLDPWD", old_pwd, env);
        char *newcwd = getcwd(NULL, 0);
        if (newcwd)
        {
            set_env_var("PWD", newcwd, env);
            free(newcwd);
        }
        else
        {
            /* Fallback if getcwd() somehow fails */
            set_env_var("PWD", candidate, env);
        }

        free(candidate);
        free(old_pwd);
        return 0;
    }

    /* ----- Case B: arg == "-" → cd $OLDPWD (after printing it) ----- */
    if (!strcmp(arg, "-"))
    {
        char *oldpwd_env = get_env_value("OLDPWD", *env);
        if (!oldpwd_env)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(old_pwd);
            return 1;
        }
        printf("%s\n", oldpwd_env);
        candidate = strdup(oldpwd_env);
        if (!candidate)
        {
            free(old_pwd);
            return 1;
        }

        status = diagnose_cd_error(candidate, 1);
        if (status != 0)
        {
            free(candidate);
            free(old_pwd);
            return 1;
        }
        if (chdir(candidate) != 0)
        {
            perror("cd");
            free(candidate);
            free(old_pwd);
            return 1;
        }

        set_env_var("OLDPWD", old_pwd, env);
        char *newcwd = getcwd(NULL, 0);
        if (newcwd)
        {
            set_env_var("PWD", newcwd, env);
            free(newcwd);
        }
        else
        {
            set_env_var("PWD", candidate, env);
        }

        free(candidate);
        free(old_pwd);
        return 0;
    }

    /* ----- Case C: arg == ".." ----- */
    /* ----- Case C: arg == ".." ----- */
    if (!strcmp(arg, ".."))
    {
        /* Try getcwd() to see if the current directory is still valid */
        char *phys = getcwd(NULL, 0);
        if (!phys)
        {
            /* 1) Error: we’re in a deleted/inaccessible directory */
            perror("cd: error retrieving current directory");
            /* No free(phys) needed—getcwd returned NULL */

            /* 2) Build new logical PWD = old logical PWD + "/.." */
            char *logic_pwd = get_env_value("PWD", *env);
            if (!logic_pwd)
                logic_pwd = old_pwd;  /* fallback */

            size_t newlen = strlen(logic_pwd) + 3; /* for "/.." + '\0' */
            char *newpwd = malloc(newlen);
            if (!newpwd)
            {
                free(old_pwd);
                return 1;
            }
            strcpy(newpwd, logic_pwd);
            strcat(newpwd, "/..");

            /* 3) UPDATE ONLY THE LOGICAL PWD—NO chdir() HERE */
            set_env_var("OLDPWD", logic_pwd, env);
            set_env_var("PWD",    newpwd,    env);

            free(newpwd);
            free(old_pwd);
            return 1;  /* return error, like bash does */
        }

        /* If getcwd() succeeded, we are now in a valid inode */
        free(phys);

        /* 4) Compute the logical parent of the old PWD and actually chdir() */
        char *logic_pwd = get_env_value("PWD", *env);
        if (!logic_pwd)
            logic_pwd = old_pwd;  /* fallback */

        char *parent = get_env_path_parent(logic_pwd);
        if (!parent)
        {
            free(old_pwd);
            return 1;
        }

        if (chdir(parent) != 0)
        {
            fprintf(stderr, "cd: no such file or directory: %s\n", parent);
            free(parent);
            free(old_pwd);
            return 1;
        }

        /* 5) On success, update OLDPWD and PWD to the real parent */
        set_env_var("OLDPWD", old_pwd, env);
        set_env_var("PWD",    parent,  env);

        free(parent);
        free(old_pwd);
        return 0;
    }


    /* ----- Case D: any other argument (absolute or relative) ----- */
    candidate = resolve_path(arg, *env);
    if (!candidate)
    {
        free(old_pwd);
        return 1;
    }

    status = diagnose_cd_error(candidate, 1);
    if (status != 0)
    {
        free(candidate);
        free(old_pwd);
        return 1;
    }
    if (chdir(candidate) != 0)
    {
        perror("cd");
        free(candidate);
        free(old_pwd);
        return 1;
    }

    set_env_var("OLDPWD", old_pwd, env);
    char *newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        set_env_var("PWD", newcwd, env);
        free(newcwd);
    }
    else
    {
        /* Fallback if getcwd() fails */
        set_env_var("PWD", candidate, env);
    }

    free(candidate);
    free(old_pwd);
    return 0;
}


 int recover_invalid_pwd(t_tree *root, t_env **env)
{
    char *old_pwd  = get_env_value("OLDPWD", *env);
    char *fake_pwd = get_env_value("PWD", *env);
    char *candidate;
    char *collapsed;
    int steps_back = 0;

    if (!old_pwd || !fake_pwd)
        return 1;

    candidate = ft_strjoin(fake_pwd, "/..");
    if (!candidate)
        return 1;

   int i = 0;
    while (candidate[i]) {
        if (candidate[i] == '/' && candidate[i + 1] == '.' && candidate[i + 2] == '.' &&
            (candidate[i + 3] == '/' || candidate[i + 3] == '\0')) {
            steps_back++;
        }
        i++;
    }


    collapsed = ft_strdup(fake_pwd);
    if (!collapsed)
    {
        free(candidate);
        return 1;
    }

    while (steps_back-- > 0)
    {
        char *parent = get_env_path_parent(collapsed);
        if (!parent)
            break;

        free(collapsed);
        collapsed = parent;

        if (access(collapsed, X_OK) == 0)
        {
            set_env_var("OLDPWD", old_pwd, env);
            set_env_var("PWD", collapsed, env);
            chdir(collapsed);
            free(candidate);
            free(collapsed);
            return 0;
        }
    }

    // If no valid collapsed path was found, try using the candidate itself
    if (access(candidate, X_OK) == 0 && chdir(candidate) == 0)
    {
        set_env_var("OLDPWD", old_pwd, env);
        set_env_var("PWD", candidate, env);
        free(candidate);
        free(collapsed);
        return 0;
    }

    free(candidate);
    free(collapsed);
    return 1;
}

