void execute_command(t_tree *root, char **env)
{
    // Basic sanity check
    if (!root || !root->data || !root->data[0])
    {
        fprintf(stderr, "Error: Empty command node\n");
        exit(EXIT_FAILURE);
    }

    // Check if the command is a builtin
    if (is_builtin(root->data[0]) == 0)
    {
        execute_builtin(root);
        exit(EXIT_SUCCESS); // Exit to avoid continuing after the builtin
    }

    // Resolve binary path
    char *binary_path = get_binary_file_path(root, env);
    if (!binary_path)
    {
        fprintf(stderr, "Error: Command not found: %s\n", root->data[0]);
        exit(EXIT_FAILURE);
    }

    // Execute the command
    execve(binary_path, root->data, env);
    perror("execve");
    free(binary_path);
    exit(EXIT_FAILURE);
}

int exec_pipe(t_tree *root, char **env)
{
    int pipefd[2];
    int status = 0;
    int pid;

    // if (!root || !root->data || !root->data[0])
    // {
    //     fprintf(stderr, "Error: Empty command node\n");
    //     return 1;
    // }

    // Create a pipe if this node has a right child
    if (root->right && pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Fork to execute the current command
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0) // Child process
    {
        // Set up stdin if there's a left pipe
        if (root->left && root->left->pipe_out != -1)
        {
            dup2(root->left->pipe_out, STDIN_FILENO);
            close(root->left->pipe_out);
        }

        // Set up stdout if there's a right child (next pipe)
        if (root->right)
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]); // Close unused read end
            close(pipefd[1]); // Close after duplicating
        }

        // Execute the current command
        execute_command(root, env);
    }

    // Parent process cleanup
    if (root->left && root->left->pipe_out != -1)
    {
        close(root->left->pipe_out); // Close after child duplicates it
    }

    if (root->right)
    {
        root->pipe_out = pipefd[0]; // Pass the read end to the right subtree
        close(pipefd[1]); // Close the write end in the parent
    }

    // Wait for the current child to prevent zombies
    waitpid(pid, &status, 0);

    // Traverse the right subtree (output)
    if (root->right)
    {
        exec_pipe(root->right, env);
    }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }
    else
    {
        return 1;
    }
}
void execute_command(t_tree *root, char **env);


static t_pipe *create_pipe()
{
    t_pipe *new_pipe = malloc(sizeof(t_pipe));
    if (!new_pipe)
    {
        return NULL;
    }
	if (pipe(new_pipe->fds) != 0)
	{
		perror("pipe");
        free(new_pipe);
	}
    new_pipe->next = NULL;
    return new_pipe;
}

// Add a pipe node to the end of the pipe list
void list_pipe_addback(t_pipe **list_pipe, t_pipe *node_pipe)
{
    if (!node_pipe)
        return;

    if (!list_pipe) {
        fprintf(stderr, "list_pipe pointer is NULL 1!\n");
        return;
    }

    if (!(*list_pipe))
	{
		
        *list_pipe = node_pipe;
    }
    else
    {
        t_pipe *temp = *list_pipe;
        if (!temp) 
		{
            fprintf(stderr, "Head pipe node is NULL!\n");
            return;
        }
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = node_pipe;
    }
}


static void free_pipe_list(t_pipe *list_pipe)
{
    t_pipe *temp;
    while (list_pipe)
    {
        close(list_pipe->fds[0]);
        close(list_pipe->fds[1]);
        temp = list_pipe;
        list_pipe = list_pipe->next;
        free(temp);
    }
}

/*
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

int setup_redirections(t_tree *root)
{
	if (!root || !root->redirections)
		return 0;
	
	if (root->redirections->type == HER_DOC)
		return setup_here_doc(root);
	return 0;
}

int execute_builtin(t_tree *root, char **env, t_env **env_list)
{
	
	if (strcmp((root->data[0]), "echo") == 0 || strcmp((root->data[0]), "ECHO") == 0)
	{
		builtin_echo(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "cd") == 0 || strcmp((root->data[0]), "CD") == 0)
	{
		cd_change_working_directory(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "pwd") == 0 || strcmp((root->data[0]), "pwd") == 0)
	{
		pwd_print_working_directory(root);
		return (0);
	}
	else if (strcmp((root->data[0]), "env") == 0 || strcmp((root->data[0]), "env") == 0)
	{
		env_environment(root, env);
		return (0);
	}
	else if (strcmp((root->data[0]), "exit") == 0 || strcmp((root->data[0]), "exit") == 0)
    {
        exit_exe(root);
		return (0);
    }
	else if (strcmp((root->data[0]), "export") == 0 || strcmp((root->data[0]), "export") == 0)
    {
        export_command_builtin(root, env_list);
		return (0);
    }
	else if (strcmp((root->data[0]), "unset") == 0 || strcmp((root->data[0]), "unset") == 0)
    {
        builtin_unset_environment(root, env_list, env);
		return (0);
    }
	else
		return (1);
}
int	is_builtin(char *command)
{
	if (strcmp((command), "echo") == 0 || strcmp((command), "ECHO") == 0)
		return (0);
	if (strcmp((command), "cd") == 0)
		return (0);
	if (strcmp((command), "env") == 0)
		return (0);
	if (strcmp((command), "exit") == 0)
		return (0);
	if (strcmp((command), "export") == 0)
		return (0);
	if (strcmp((command), "pwd") == 0)
		return (0);
	if (strcmp((command), "unset") == 0)
		return (0);
	return (1);
}
void free_tree_exe(t_tree *root) {
    if (!root)
        return;
    
    // Free left and right subtrees recursively
    free_tree_exe(root->left);
    free_tree_exe(root->right);
    
    // Free command data if present
    if (root->data) {
        for (int i = 0; root->data[i] != NULL; ++i)
            free(root->data[i]);
        free(root->data);
    }
    
    // Free the root node itself
    free(root);
}

void cleanup(int *pipefd, int input_fd, t_tree *root) 
{
	if (pipefd) 
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (root)
		free_tree_exe(root);
}

int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd)
{
    if (!root)
        return 1;

    if (root->type == BLOCK)
        return execute_command(root, env, env_list) , 0;

    if (root->type == PIPE)
        return exec_pipe(root, env, input_fd, env_list);

    if (root->type == OP_AND)
    {
        int status = exec_tree(root->left, env, env_list, STDIN_FILENO);
        if (status == 0)
            return exec_tree(root->right, env, env_list, STDIN_FILENO);
        return status;
    }

    if (root->type == OP_OR)
    {
        int status = exec_tree(root->left, env, env_list, STDIN_FILENO);
        if (status != 0)
            return exec_tree(root->right, env, env_list, STDIN_FILENO);
        return status;
    }

    return 1;
}


int exec_pipe(t_tree *root, char **env, int input_fd, t_env **env_list)
{
    int pipefd[2];
    pid_t pid_left, pid_right;
    int status = 0;

    if (!root || root->type != PIPE)
        return 1;

    // Create pipe
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Fork left child (command before the pipe)
    pid_left = fork();
    if (pid_left < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    else if (pid_left == 0)
    {
        // Left child: Set input and output
        if (input_fd != STDIN_FILENO)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        dup2(pipefd[1], STDOUT_FILENO); // Output to pipe write end
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]); // Close after dup2

        // Execute left subtree
        if (root->left->type == PIPE)
            exec_pipe(root->left, env, STDIN_FILENO, env_list);
        else
            execute_command(root->left, env, env_list);
        exit(EXIT_FAILURE); // Should not reach here
    }

    // Fork right child (command after the pipe)
    pid_right = fork();
    if (pid_right < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        return 1;
    }
    else if (pid_right == 0)
    {
        // Right child: Set input
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Input from pipe read end
        close(pipefd[0]); // Close after dup2
        if (input_fd != STDIN_FILENO)
            close(input_fd);

        // Execute right subtree
        if (root->right->type == PIPE)
            exec_pipe(root->right, env, STDIN_FILENO, env_list);
        else
            execute_command(root->right, env, env_list);
        exit(EXIT_FAILURE); // Should not reach here
    }

    // Parent: Clean up and wait
    close(pipefd[0]);
    close(pipefd[1]);
    if (input_fd != STDIN_FILENO)
        close(input_fd);

    waitpid(pid_left, &status, 0);
    waitpid(pid_right, &status, 0);

    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}


void execute_command(t_tree *root, char **env, t_env **env_list)
{
    char *binary_path;

    if (!root || !root->data || !root->data[0])
    {
        fprintf(stderr, "Error: Empty command node\n");
        free_tree_exe(root);
        exit(EXIT_FAILURE);
    }

    // Execute built-in command
    if (is_builtin(root->data[0]) == 0)
    {
        execute_builtin(root, env, env_list);
        free_tree_exe(root);
        exit(EXIT_SUCCESS);
    }

    // Execute external command
    binary_path = get_binary_file_path(root, env);
    if (!binary_path)
    {
        fprintf(stderr, "Error: Command not found: %s\n", root->data[0]);
        free_tree_exe(root);
        exit(EXIT_FAILURE);
    }

    execve(binary_path, root->data, env);
    perror("execve");
    free(binary_path);
    free_tree_exe(root);
    exit(EXIT_FAILURE);
}
int execution(t_tree *root, char **env, t_env **env_list)
{
    int status = 0;
    pid_t pid;

    root->is_forked = 0;
    if (!root)
        return (1);
    if (root->type == PIPE)
    {
        root->is_forked = 1;
        status = exec_pipe(root, env, STDIN_FILENO, env_list);
        free_tree_exe(root);
        return (status);
    }
    if (root->type == OP_AND)
    {
        status = exec_tree(root->left, env, env_list, STDIN_FILENO);
        if (status == 0)
            exec_tree(root->right, env, env_list, STDIN_FILENO);
    }
    if (root->type == OP_OR)
    {
        status = exec_tree(root, env,  env_list, STDIN_FILENO);
        if (status != 0)
            exec_tree(root->right, env, env_list, STDIN_FILENO);
    }
    if (is_builtin(root->data[0]) == 0)
    {
        execute_builtin(root, env, env_list);
        free_tree_exe(root);
        return (0);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free_tree_exe(root);
        return (1);
    }
    else if (pid == 0)
    {
        execute_command(root, env, env_list);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    free_tree_exe(root);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return (1);
}






*/