#include "../../include/execution.h"

static char *expand_home(char *arg, t_env *env)
{
    char *home;
    size_t needed;
    char *res;

    home = get_env_value("HOME", env);
    if (!home)
        return (NULL);

    needed = ft_strlen(home) + ft_strlen(arg);
    res = malloc(needed + 1);
    if (!res)
        return (NULL);

    strcpy(res, home); // 
    strcat(res, arg + 1); //
    return (res);
}

static char *resolve_path(char *arg, t_env *env)
{
    char *oldpwd;

    if (!arg)
        return (NULL);

    if (arg[0] == '~')
        return (expand_home(arg, env));

    if (!ft_strcmp(arg, "-"))
    {
        oldpwd = get_env_value("OLDPWD", env);
        if (!oldpwd)
            return (NULL);
        printf("%s\n", oldpwd);
        return (ft_strdup(oldpwd));
    }
    return ft_strdup(arg);
}

static int check_argument(t_tree *root)
{
    if (root && root->data && root->data[0] && root->data[1] && root->data[2])
    {
        perror("cd: too many arguments\n");
        return (1);
    }
    return (0);
}

int has_one_case_in_cd(t_env **env, char *old_pwd, char *candidate)
{
    char *newcwd;
    char *home ;

    home = get_env_value("HOME", *env);
    if (!home)
        return (perror("cd: HOME not set\n"),free(old_pwd), 1);
    candidate = strdup(home);
    if (!candidate)
        return (free(old_pwd), 1);
    if ( diagnose_cd_error(candidate, 1) != 0)
        return (free(candidate),free(old_pwd), 1);
    if (chdir(candidate) != 0)
        return (perror("cd"),free(candidate),free(old_pwd), 1);
    set_env_var("OLDPWD", old_pwd, env);
    newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        set_env_var("PWD", newcwd, env);
        free(newcwd);
    }
    else
        set_env_var("PWD", candidate, env);
    return (free(candidate),free(old_pwd), 0);
}


int apply_cd_with_hyphen(t_env **env, char *old_pwd, char *candidate)
{
    char *newcwd;
    char *oldpwd_env;

    oldpwd_env = get_env_value("OLDPWD", *env);
    if (oldpwd_env == NULL)
    {
        ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
        return (free(old_pwd), 1);
    }
    candidate = ft_strdup(oldpwd_env);
    if (!candidate)
        return (free(old_pwd), 1);
    if (diagnose_cd_error(candidate, 1) != 0 || chdir(candidate) != 0)
        return ( free(candidate),free(old_pwd), 1);
    set_env_var("OLDPWD", old_pwd, env);
    newcwd = getcwd(NULL, 0);
    if (newcwd)
    {
        set_env_var("PWD", newcwd, env);
        free(newcwd);
    }
    else
        set_env_var("PWD", candidate, env);
    return (free(candidate), free(old_pwd), 0);
}


int cd_change_working_directory(t_tree *root, t_env **env)
{
    char *old_pwd;
    char *arg;
    char *candidate;

    if (check_argument(root) || is_illegal_cd_arg(root->data[1]))
        return (1);
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        old_pwd = ft_strdup(get_env_value("PWD", *env));
        if (!old_pwd)
            return (1);
    }
    arg = get_arg_cd(root->data[1]);
    if (!arg)
        return has_one_case_in_cd(env, old_pwd, NULL);
    if (!ft_strcmp(arg, "-"))
        return free(arg), apply_cd_with_hyphen(env, old_pwd, NULL);
    if (!ft_strcmp(arg, ".."))
        return free(old_pwd), apply_cd_with_double_dots(root, env, arg);
    candidate = resolve_path(arg, *env);
    if (!candidate || chdir(candidate) != 0)
    {
        free(old_pwd);
        return (diagnose_cd_error(candidate, 1), free(candidate),free(arg), 1);
    }
    set_env_var("OLDPWD", old_pwd, env);
    return (update_pwd(env), free(arg), free(old_pwd), free(candidate), 0);
}
