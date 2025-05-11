#include "../include/execution.h"

#include <string.h>
//todo remove this 
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
    // if (is_builtin(root->data[0]) == 0)
    //     return execute_builtin(root->data[0]);
	t_env *env1 = NULL ;
	env_generate(&env1, env);

    pid_t pid = fork();
    if (pid == 0)
    {
        // In child process
		expand(env1, root);
        execve("/bin/ls", root->data, env);
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

void execution(t_tree *root, char **env)
{
	if (root == NULL)
		return ;
	execute_command(root, env);
	// execution(root->left);
	// if (root->data && is_buit_in_function(root->data[0]))
	// 	printf("is not built-in fucntion just command \n");
	// // printf("%s\n", *root->data);
	// execution(root->right);

}