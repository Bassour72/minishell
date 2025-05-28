#include "../../include/execution.h"

int env_environment(t_tree *root, char **env, t_env *env_list)
{
    // t_env *env_list = NULL;

    if (root->data[1])
    {
        printf("env: '%s': No such file or directory\n", root->data[1]);
        return (127);
    }
    // env_generate(&env_list, env);
    print_env(env_list); 
    // free_env_list(env_list);
    return (0);
}
