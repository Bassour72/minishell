#include "../include/execution.h"

#include <string.h>
//todo remove this 
//fixme 


int execute_builtin(t_tree *root)
{
	if (strcmp((root->data[0]), "echo") == 0 || strcmp((root->data[0]), "ECHO") == 0)
	{
		builtin_echo(root);
		return (0);
	}
	if (strcmp((root->data[0]), "cd") == 0 || strcmp((root->data[0]), "CD") == 0)
		return (0);
	if (strcmp((root->data[0]), "env") == 0 || strcmp((root->data[0]), "env") == 0)
		return (0);
	if (strcmp((root->data[0]), "exit") == 0 || strcmp((root->data[0]), "exit") == 0)
		return (0);
	if (strcmp((root->data[0]), "export") == 0 || strcmp((root->data[0]), "export") == 0)
		return (0);
	if (strcmp((root->data[0]), "pwd") == 0 || strcmp((root->data[0]), "pwd") == 0)
		return (0);
	if (strcmp((root->data[0]), "unset") == 0 || strcmp((root->data[0]), "unset") == 0)
		return (0);
	return (1);
}
int	is_builtin(char *command)
{
	if (strcmp((command), "echo") == 0 || strcmp((command), "ECHO") == 0)
		return (0);
	if (strcmp((command), "cd") == 0 || strcmp((command), "CD") == 0)
		return (0);
	if (strcmp((command), "env") == 0 || strcmp((command), "env") == 0)
		return (0);
	if (strcmp((command), "exit") == 0 || strcmp((command), "exit") == 0)
		return (0);
	if (strcmp((command), "export") == 0 || strcmp((command), "export") == 0)
		return (0);
	if (strcmp((command), "pwd") == 0 || strcmp((command), "pwd") == 0)
		return (0);
	if (strcmp((command), "unset") == 0 || strcmp((command), "unset") == 0)
		return (0);
	return (1);
}
int execute_command(t_tree *root, char **env)
{
	t_env *env1 = NULL ;
	char *binary_path;
    if (is_builtin(root->data[0]) == 0)
        return execute_builtin(root);
	//env_generate(&env1, env);

    pid_t pid = fork();
    if (pid == 0)
    {
		binary_path = get_binary_file_path(root, env);
        // In child process
		expand(env1, root);
        execve(binary_path, root->data, env);
		free_tree(root);
		free(binary_path);
        perror("execve");
        exit(1);
    }
    else if (pid > 0)
    {
        // In parent process
        waitpid(pid, NULL, 0);
    }
    return 0;
}


int execution(t_tree *root, char **env)
{
    if (root == NULL)
        return 1;
    
    if (root->type == PIPE)
    {
        printf("Processing pipe\n");
        execution(root->left, env);
        execution(root->right, env);
    }
    else
    {
		printf("Processing command\n");
       // execute_command(root, env);
    }

    return 1;
}
