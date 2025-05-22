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