#include "../../include/execution.h"
#include <sys/stat.h>
#include <errno.h>

#define DIR_STATUS_NO_PER 11
#define DIR_STATUS_NOT_EXI 12

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

int recover_invalid_pwd(t_env **env, char *fake_pwd);

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
       printf("has_one_case_in_cd\n\n");
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
    printf("apply_cd_with_hyphen - \n");
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
    newcwd = getcwd(NULL, 0);
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

int apply_cd_with_double_dots(t_tree *root,t_env **env, char *old_pwd, char *candidate,  char *arg)
{
    char *phys = getcwd(NULL, 0);
    perror("cd: error retrieving current directory");
    sleep(1);
    perror(phys);
    perror("\n\n\n");
    if (!phys)
    {
        perror("cd: error retrieving current directory");
        // char **logic_pwd = ft_split(arg, '/');
        // int i = 0;
        // while (logic_pwd[i] != NULL)
        // {
        //     /* code */
        //     recover_invalid_pwd(env,arg);
        //     i++;
        // }
        //todo I  HAVE LEAKS HERE FOR THE logic_pwd
        return 1;
    }
    free(phys);
    printf("here_the_first_01\n\n");
    char *logic_pwd = get_env_value("PWD", *env);
    if (!logic_pwd)
        logic_pwd = old_pwd;
      printf("here_the_first_02\n\n");
    char *parent = get_env_path_parent(logic_pwd);
    if (!parent)
    {
        free(old_pwd);
        return 1;
    }
      printf("here_the_first_03\n\n");
    if (chdir(parent) != 0)
    {
        fprintf(stderr, "cd: no such file or directory: %s\n", parent);
        free(parent);
        free(old_pwd);
        return 1;
    }
    /* 5) On success, update OLDPWD and PWD to the real parent */
    printf("here_the_first_04\n\n");
    set_env_var("OLDPWD", old_pwd, env);
    set_env_var("PWD",    parent,  env);
    free(parent);
    free(old_pwd);
    return 0;
}

int cd_change_working_directory(t_tree *root, t_env **env)
{
    char *old_pwd;
    char *env_pwd;
    char *arg;
    char *candidate;
    int status;
    if (check_argument(root) || is_valid_cd_arg(root->data[1]))
    {
        return 1;
    }
    old_pwd = getcwd(NULL, 0);
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
    arg       = get_arg_cd(root->data[1]);
    candidate = NULL;
    status;
    /* ----- Case A: no argument → cd $HOME ----- */
    
    if (!arg)
        return has_one_case_in_cd(env, old_pwd, candidate);
    /* ----- Case D: any other argument (absolute or relative) ----- */
    if (!strcmp(arg, "-"))
        return (apply_cd_with_hyphen(env, old_pwd, candidate));

    if (!strcmp(arg, ".."))
    {
        return (apply_cd_with_double_dots(root,env, old_pwd, candidate, arg));
    }
    if (!strncmp(arg, "/", 1))
    {
        if (chdir("/") != 0)
        {
            perror("cd");
            free(candidate);
            free(old_pwd);
            return 1;
        }
        set_env_var("OLDPWD", old_pwd, env);
        set_env_var("PWD", "/", env);
        return 0;
    }
    
    candidate = resolve_path(arg, *env);
    if (!candidate)
    {
        free(old_pwd);
        return 1;
    }
    status = diagnose_cd_error(candidate, 1);
    if (status != 0)
    {
        perror("cd: error retrieving current directory does here ");
        recover_invalid_pwd(env, arg);
        //todo I  HAVE LEAKS HERE FOR THE logic_pwd
        return 1;
    }
    printf("HERE FOR PRINTF TO DEBUG \n\n");
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


char *remove_trailing_slash(const char *path) 
{
    if (!path)
        return NULL;

    int len = strlen(path);
    while (len > 1 && path[len - 1] == '/')
        len--;

    char *cleaned = malloc(len + 1);
    if (!cleaned)
        return NULL;

    memcpy(cleaned, path, len);
    cleaned[len] = '\0';
    return cleaned;
}


int recover_invalid_pwd(t_env **env, char *new_pwd)
{
    char *fake_pwd;
    int steps_back;
    char *parent;
    char *new_parent;
    int i;
    if (!new_pwd)
        return 1;
    steps_back = 0;
    i = 0;
    while (new_pwd[i])
    {
        if (new_pwd[i] == '.' && new_pwd[i + 1] != '\0' && new_pwd[i + 1] == '.')
            steps_back++;
        i++;
    }
    fake_pwd = get_env_value("PWD", *env);
    fake_pwd = ft_strdup(fake_pwd);
    if (!fake_pwd)
    {
        return 1;
    }
    parent = fake_pwd;
    while (steps_back-- > 0)
    {
        new_parent = get_env_path_parent(parent);
        if (!new_parent)
        {
            free(parent);
            free(fake_pwd);
            return 1;
        }
        free(parent);
        parent = new_parent;
    }
    if (access(parent, X_OK) == 0 && chdir(parent) == 0)
    {
        set_env_var("PWD", parent, env);
        free(parent);
       free(fake_pwd);
        return 0;
    }
    free(parent);
   free(fake_pwd);
    return 1;
}


