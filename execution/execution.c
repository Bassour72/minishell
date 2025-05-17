#include "../include/execution.h"

#include <string.h>
#include <stdlib.h>
//todo remove this 
//fixme 

int	read_her_doc(t_tree *root)
{
	
}
int	set_up_input(t_tree *root)
{
	if (!root)
		return (1);
	if (root->type == HER_DOC)
	{

	}
}

int execute_builtin(t_tree *root)
{
	
	if (strcmp((root->data[0]), "echo") == 0 || strcmp((root->data[0]), "ECHO") == 0)
	{
		builtin_echo(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "cd") == 0 || strcmp((root->data[0]), "CD") == 0)
	{
		cd_change_current_directory(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "pwd") == 0 || strcmp((root->data[0]), "pwd") == 0)
	{
		pwd_print_working_directory(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "env") == 0 || strcmp((root->data[0]), "env") == 0)
		return (0);
	else if (strcmp((root->data[0]), "exit") == 0 || strcmp((root->data[0]), "exit") == 0)
		return (0);
	else if (strcmp((root->data[0]), "export") == 0 || strcmp((root->data[0]), "export") == 0)
		return (0);
	else if (strcmp((root->data[0]), "unset") == 0 || strcmp((root->data[0]), "unset") == 0)
		return (0);
	else
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
int execute_command(t_tree *root, char **env, t_pipe *list_pipe)
{
	t_env *env1 = NULL ;
	char *binary_path;
    // if (is_builtin(root->data[0]) == 0)
    //     return execute_builtin(root);
	//env_generate(&env1, env);

    pid_t pid = fork();
    if (pid == 0)
    {
		printf("=============================%d\n", pid);
		binary_path = get_binary_file_path(root, env);
        // In child process
		expand(env1, root);
		close(list_pipe->pipe[0]);
		dup2(1, list_pipe->pipe[1]);
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

t_pipe	*create_pipe(int number)
{
	t_pipe *node_pipe;
	node_pipe = (t_pipe*)malloc(sizeof(t_pipe));
	if (!node_pipe)
		return (NULL);
	if (pipe(node_pipe->pipe) != 0)
	{
		free(node_pipe);
		return (NULL);
	}
	node_pipe->test = ft_itoa(number);
	node_pipe->next = NULL;
	return (node_pipe);
}

void	list_pipe_addback(t_pipe **list_pipe, t_pipe *node_pipe)
{
	if (!node_pipe)
		return ;
	if (!(*list_pipe))
	{
		*list_pipe = node_pipe;
	}
	else
	{
		t_pipe * temp;
		temp = *list_pipe;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = node_pipe;
	}
}

int	exec_pipe(t_tree *root, char **env, int i)
{
	if (root == NULL)
		return (1);

	t_pipe *list_pipe = NULL;
	exec_pipe(root->left, env, i);

	//printf("[exec pipe ] Processing pipe and  multips commands left\n");
	list_pipe_addback(&list_pipe, create_pipe(i++));
	if (root->data && root->data[0])
	{
		printf("[%s][%s]\n", root->data[0], list_pipe->test);
		execute_command(root, env, list_pipe);
		list_pipe = list_pipe->next;
		
	}
	exec_pipe(root->right, env, i);
	//printf("[exec pipe ] Processing pipe and  multips commands right\n");
}

int execution(t_tree *root, char **env)
{
    if (root == NULL)
        return 1;
    if (root->type == PIPE)
    {
        printf("[execution fucntion] Processing pipe and  multips commands\n");
		exec_pipe(root, env, 1);
    }

	else if (is_builtin(root->data[0]) == 0)
	{
		printf("Processing built-in-command and no pipe not binary command just fucntion\n");
        return execute_builtin(root);
	}
    else
    {
		printf("Processing command single command\n");
        //execute_command(root, env);
    }

    return 1;
}
