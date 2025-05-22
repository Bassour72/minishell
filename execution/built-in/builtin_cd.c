#include "../../include/execution.h"

typedef struct s_env_exec {
    char *name;
    char *value;
    struct s_env_exec *next;
} t_env_exec;

//t_env_exec *env_list = NULL; // Global variable for environment list

void set_env_var(char *name, char *value) 
{
    static t_env_exec *env_list = NULL;
    t_env_exec *current = env_list;

    //todo Look for existing variable
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0) 
        {
            free(current->value);
            current->value = strdup(value);
            if (current->value == NULL) {
                perror("strdup");
                return;
            }
            return;
        }
        current = current->next;
    }

    //todo Create a new node
    t_env_exec *new_env = malloc(sizeof(t_env_exec));
    if (!new_env) 
    {
        perror("malloc");
        return;
    }
    new_env->name = strdup(name);
    new_env->value = strdup(value);
    if (!new_env->name || !new_env->value) {
        perror("strdup");
        free(new_env->name);
        free(new_env->value);
        free(new_env);
        return;
    }
    new_env->next = env_list;
    env_list = new_env; //todo Add to front of list
}

int cd_change_working_directory(t_tree *root) 
{
    char *dir;
    char cwd[1024];
    char *expanded_dir = NULL;

    if (getcwd(cwd, sizeof(cwd)) == NULL) 
    {
        perror("getcwd");
        return 1;
    }
    if (root == NULL || root->data == NULL) 
    {
        fprintf(stderr, "cd: invalid command structure\n");
        return 1;
    }

    if (root->data[1] == NULL) 
    {
        dir = getenv("HOME");
        if (dir == NULL) 
         {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    } 
    else if (root->data[2] != NULL) 
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    } 
    else 
    {
        //todo Handle tilde expansion
        if (root->data[1][0] == '~' && (root->data[1][1] == '/' || root->data[1][1] == '\0'))
        {
            char *home = getenv("HOME");
            if (home == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
            //todo Calculate length: home + rest of path (skip ~) + null terminator
            size_t len = strlen(home) + strlen(root->data[1]) + 1;
            expanded_dir = malloc(len);
            if (!expanded_dir) {
                perror("malloc");
                return 1;
            }
            strcpy(expanded_dir, home);
            if (root->data[1][1] != '\0') { //todo If there's a path after ~
                strcat(expanded_dir, root->data[1] + 1); //todo Skip ~
            }
            dir = expanded_dir;
        }
         else 
        {
            dir = root->data[1];
        }
    }

    if (chdir(dir) == 0)
    {
        set_env_var("OLDPWD", cwd);
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            set_env_var("PWD", cwd);
        else
            perror("getcwd"); //todo Still return 0 since chdir succeeded
        free(expanded_dir); //todo Free allocated memory
        return 0;
    } else 
    {
        perror("cd");
        free(expanded_dir); //todo Free allocated memory
        return 1;
    }
}