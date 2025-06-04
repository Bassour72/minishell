#include "../../include/execution.h"

int print_environment(t_env *env_list)
{
    t_env *tmp_env;

    if (!env_list)
        return (1);

    tmp_env = env_list;
    while (tmp_env)
    {
        if (tmp_env->key && tmp_env->value)
        {
            printf("%s=%s\n", tmp_env->key, tmp_env->value);
        }
        tmp_env = tmp_env->next;
    }
    return (0);
}

int env_environment(t_tree *root, char **env, t_env *env_list)
{
    if (root->data[1])
    {
        // printf("env: '%s': No such file or directory\n", root->data[1]);
        return (127);
    }
    return print_environment(env_list);
}

