#include "../../include/execution.h"
#include <sys/stat.h>
#include <errno.h>

#define DIR_STATUS_NO_PER 11
#define DIR_STATUS_NOT_EXI 12

int apply_cd_with_double_dots(t_tree *root, t_env **env, char *arg);

void update_pwd(t_env **env);
int is_valid_cd_arg(const char *arg)
{
    if (arg == NULL)
        return 0;
    int i = 0;
    int count_dot = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == '.')
        {
            count_dot++;
            if (count_dot == 3)
            {
                fprintf(stderr, "cd: %s: invalid argument (three consecutive dots)\n", arg);
                return 1;
            }
        }
        else
        {
            count_dot = 0;
        }
        i++;
    }
    i = 0;
    int count_hyphen = 0;
    while (arg[i] != '\0')
    {
        if (arg[i] == '-')
        {
            count_hyphen++;
            if (count_hyphen == 3)
            {
                fprintf(stderr, "cd: %s: invalid argument (three consecutive hyphens)\n", arg);
                return 1;
            }
        }
        else
        {
            count_hyphen = 0;
        }
        i++;
    }
    i = 0;
    while (arg[i] != '\0' && arg[i + 1] != '\0')
    {
        if (arg[i] == '.' && arg[i + 1] == '-')
        {
            fprintf(stderr, "cd: %s: invalid argument (\".-\" not allowed)\n", arg);
            return 1;
        }
        if (arg[i] == '-' && (arg[i + 1] == '.' || arg[i + 1] == '/'))
        {
            if (arg[i + 1] == '.')
                fprintf(stderr, "cd: %s: invalid argument (\"-.\" not allowed)\n", arg);
            else
                fprintf(stderr, "cd: %s: invalid argument (\"-/\" not allowed)\n", arg);
            return 1;
        }
        i++;
    }

    return 0;
}


char * get_arg_cd(const char *arg)
{
    if (!arg)
        return NULL;
    //if (strcmp(arg, "-") == 0)
        return ft_strdup(arg);
    // struct stat sb;
    // if (stat(arg, &sb) == 0 && S_ISDIR(sb.st_mode))
    //     return ft_strdup(arg);
    // return NULL;
}

char *get_env_value(char *key, t_env *env)
{
    while (env)
    {
        if (!ft_strcmp(env->key, key))
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
            char *new_value = ft_strdup(value); // Duplicate first
            if (new_value)                  // Check allocation success
            {
                free(tmp->value);           // Free old value
                tmp->value = new_value;     // Assign new value
            }
            return;
        }
        tmp = tmp->next;
    }
    t_env *new = malloc(sizeof(t_env));
    if (!new)
        return;
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->exported = 1;
    new->is_remove = 1;
    new->next = *env;
    *env = new;
}

char *get_env_path_parent(const char *path)
{
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
    char *oldpwd;

    if (!arg)
        return NULL;

    if (arg[0] == '~')
        return expand_home(arg, env);

    if (!strcmp(arg, "-"))
    {
        oldpwd = get_env_value("OLDPWD", env);
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
    if (access(path, X_OK) != 0)
    {
        if (print_error)
            fprintf(stderr, "cd: permission denied: %s\n", path);
        return (DIR_STATUS_NO_PER);
    }
    return (0);
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

int has_one_case_in_cd(t_env **env, char *old_pwd, char *candidate)
{
    int status;
    char *newcwd;
    char *home ;

    home = get_env_value("HOME", *env);
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
    // idea If you have cd without arg this case you should return to home directory 
    // in the bad case if no permission or remove parent dir no warrning
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
    newcwd = getcwd(NULL, 0);
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


int apply_cd_with_hyphen(t_env **env, char *old_pwd, char *candidate)
{
    int status;
    char *newcwd;
    char *oldpwd_env;

    oldpwd_env = get_env_value("OLDPWD", *env);
    if (!oldpwd_env)
    {
        fprintf(stderr, "cd: OLDPWD not set\n");
        free(old_pwd);
        return 1;
    }
    candidate = strdup(oldpwd_env);
    if (!candidate)
        return (free(old_pwd), 1);
    status = diagnose_cd_error(candidate, 1);
    if (status != 0 || chdir(candidate) != 0)
        return ( free(candidate),free(old_pwd), 1);
    set_env_var("OLDPWD", old_pwd, env);
    newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        printf("newcwd [%s]\n\n\n", newcwd);
        set_env_var("PWD", newcwd, env);
        free(newcwd);
    }
    else
        set_env_var("PWD", candidate, env);
    free(candidate);
    free(old_pwd);
    return 0;
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

int cd_change_working_directory(t_tree *root, t_env **env)
{
    char *old_pwd;
    char *arg;
    char *candidate;

    if (check_argument(root) || is_valid_cd_arg(root->data[1]))
        return 1;
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        old_pwd = ft_strdup(get_env_value("PWD", *env));
        if (!old_pwd)
            return 1;
    }
    arg = get_arg_cd(root->data[1]);
    if (!arg)
        return has_one_case_in_cd(env, old_pwd, NULL);
    if (!strcmp(arg, "-"))
        return free(arg), apply_cd_with_hyphen(env, old_pwd, NULL);
    if (!strcmp(arg, ".."))
        return apply_cd_with_double_dots(root, env, arg);
    candidate = resolve_path(arg, *env);
    if (!candidate || chdir(candidate) != 0)
    {
        diagnose_cd_error(candidate, 1);
        free(candidate);
        free(old_pwd);
        free(arg);
        return 1;
    }
    set_env_var("OLDPWD", old_pwd, env);
    update_pwd(env);
    free(candidate);
    free(old_pwd);
    free(arg);
    return 0;
}

int apply_cd_with_double_dots(t_tree *root, t_env **env, char *arg)
{
    char *logic_pwd;
    char *parent;
    char *temp;
    char *old_pwd;

    chdir(arg);
    old_pwd = getcwd(NULL, 0);
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
    return (free(arg) ,0);
}
//449 line