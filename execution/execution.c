#include "../include/execution.h"



//todo remove this 
//fixme 

void write_here_doc(int fd, char *limiter_nl)
{
    char *line;

    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, limiter_nl) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);  // Ensure each line is properly terminated
        free(line);
    }
}

int setup_here_doc(t_tree *root)
{
    char tmp_file[] = "/tmp/heredocXXXXXX";
    int in_fd = mkstemp(tmp_file);
    int out_fd;

    if (in_fd < 0)
    {
        perror("Error creating temporary file");
        exit(EXIT_FAILURE);
    }

    write_here_doc(in_fd, root->redirections->data);
    close(in_fd);

    // Reopen for reading and unlink immediately to clean up
    out_fd = open(tmp_file, O_RDONLY);
    if (out_fd < 0)
    {
        perror("Error reopening temporary file");
        exit(EXIT_FAILURE);
    }
    unlink(tmp_file);

    // Set as the current process's stdin
    if (dup2(out_fd, STDIN_FILENO) < 0)
    {
        perror("Error duplicating file descriptor");
        close(out_fd);
        exit(EXIT_FAILURE);
    }
    close(out_fd);
    return 0;
}

int set_up_input(t_tree *root)
{
    if (!root || !root->redirections)
        return 0;
    
    if (root->redirections->type == HER_DOC)
        return setup_here_doc(root);
    return 0;
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

void print_treeeee(t_tree *node, int level)
{
    if (!node)
        return;
    for (int i = 0; i < level; i++)
        printf("  ");
	if (node->data && node->data[0])
    	printf("Node: %s, Type: %d\n", node->data[0], node->type);
	else
		printf("Node: %s, Type: %d\n", "pipe", node->type);
    print_tree(node->left, level + 1);
    print_tree(node->right, level + 1);
}

int exec_pipe(t_tree *root, char **env, int input_fd)
{
    int pipefd[2];
    pid_t pid;
    int status;

    if (!root)
        return 1; // Null tree

    if (root->type == PIPE)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return 1;
        }

        // Fork left child
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Left child executes left subtree
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);

            exec_pipe(root->left, env, STDIN_FILENO);
            exit(1);
        }

        // Fork right child
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Right child executes right subtree
            close(pipefd[1]);
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            exec_pipe(root->right, env, STDIN_FILENO);
            exit(1);
        }

        // Parent closes fds and input_fd if needed
        close(pipefd[0]);
        close(pipefd[1]);
        if (input_fd != STDIN_FILENO)
            close(input_fd);

        // Wait for both children
        while (wait(&status) > 0);
        free_tree(root);

        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    else
    {
        // Leaf node: single command execution
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        execute_command(root, env);  // Your execve or wrapper here
        exit(1); // Should never reach here if exec succeeds
    }
}



void execute_command(t_tree *root, char **env)
{
	char *binary_path;

    if (!root || !root->data || !root->data[0])
    {
		//todo for debugging
        fprintf(stderr, "Error: Empty command node\n");
		// here should free all resources you use in the minishell
        exit(EXIT_FAILURE);
    }
    if (is_builtin(root->data[0]) == 0)
    {
        execute_builtin(root);
		fprintf(stderr, "Error: Command not found: %s\n", root->data[0]);
		// here not exit beacuse you to return the status of the execution fo funtions
        exit(EXIT_SUCCESS); // Exit to avoid continuing after the builtin
    }
    binary_path = get_binary_file_path(root, env);
	
    if (!binary_path)
    {
			//todo for debugging
        fprintf(stderr, "Error: Command not found: %s\n", root->data[0]);
		// free all resources and close all pipes and fds
        exit(EXIT_FAILURE);
    }
    execve(binary_path, root->data, env);
    perror("execve");
	// free all resources and close all use pipe and fds
    free(binary_path);
    exit(EXIT_FAILURE);
}

int execution(t_tree *root, char **env)
{
    int status = 0;
	t_pipe *pipe_list  = NULL;
    if (!root)
        return 1;
    if (root->type == PIPE)
    {
        printf("[execution function] Processing pipe and multiple commands\n");
		//print_treeeee(root, 0);
        return exec_pipe(root, env, STDIN_FILENO);
    }
    if (is_builtin(root->data[0]) == 0)
    {
        printf("Processing built-in-command and no pipe, not binary command, just function\n");
        execute_builtin(root);
        return 0;
    }

    // Set up heredoc if present
    // if (root->redirections && root->redirections->type == HER_DOC)
    // {
    //     if (set_up_input(root) != 0)
    //     {
    //         fprintf(stderr, "Error: Failed to set up heredoc\n");
    //         return 1;
    //     }
    // }

    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        execute_command(root, env);
        exit(1);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    else
        return 1;
}



