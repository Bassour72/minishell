#include "../../../include/execution.h"

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