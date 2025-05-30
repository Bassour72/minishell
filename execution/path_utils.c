#include "../include/execution.h"
#include <string.h>

static char *extract_path_variable(t_env **env_list)
{
    t_env *env = *env_list;
    while (env != NULL)
    {
        if (ft_strcmp(env->key, "PATH") == 0)
        {
            return (ft_strdup(env->value));
        }
        env = env->next;
    }
    return (NULL);
}

int has_point(char *command)
{
    if (access(command, X_OK) == 0)
    {
        return (0);
    }
    else
        return (1);
}

static char *join_binary_path_with_command(char *command, char *binary_path)
{
    char **binaries_path;
    char *full_path;
	char *temp;
    int i = 0;

    if (!has_point(command))
        return (ft_strdup(command));
    binaries_path = ft_split(binary_path, ':');
    if (binaries_path == NULL)
        return (NULL);
    while (binaries_path[i] != NULL)
    {
        full_path = ft_strjoin(binaries_path[i], "/");
        temp = ft_strjoin(full_path, command);
        free(full_path);
        if (access(temp, X_OK) == 0)
        {
            //todo Free the entire binaries_path array
			// fixme write fucntion for clean memory leaks
            i = 0;
            while (binaries_path[i] != NULL)
                free(binaries_path[i++]);
            free(binaries_path);
            return (temp);
        }
        free(temp);
        i++;
    }

    //todo Free the binaries_path array if no valid path is found
    i = 0;
    while (binaries_path[i] != NULL)
        free(binaries_path[i++]);
    free(binaries_path);
    return (NULL);
}

char *get_binary_file_path(t_tree *root, char **env, t_env **env_list)
{
    char *binary_path;

     if (!root || !root->data || !root->data[0])
    {
        fprintf(stderr, "Error: Empty command node\n"); //todo change fprint
        return NULL;
    }

    binary_path = extract_path_variable(env_list);
    if (binary_path == NULL)
        return (NULL);
    return (join_binary_path_with_command(root->data[0], binary_path));
}
