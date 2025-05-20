#include "../include/execution.h"



//todo remove this 
//fixme 

void free_tree_exe(t_tree *root);

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

int execute_builtin(t_tree *root, char **env)
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
	{
		env_environment(root, env);
		return (0);
	}
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

int exec_pipe(t_tree *root, char **env, int input_fd) 
{
    int pipefd[2] = {-1, -1};
    pid_t pid_left, pid_right;
    int status = 0;
    
    if (!root)
        return 1;
    
    if (root->type == PIPE) 
    {
        // Create pipe
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
           // free_tree_exe(root);
            return 1;
        }
        
        // Fork left child (command before the pipe)
        pid_left = fork();
        if (pid_left < 0) 
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            //free_tree_exe(root);
            return 1;
        } 
        else if (pid_left == 0) 
        {
            // Child process for left command
            if (input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            
            // Redirect stdout to pipe write end
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);  // Close unused pipe read end
            close(pipefd[1]);  // Close after dup2
            
            // Execute left subtree
            if (root->left->type == PIPE)
                exec_pipe(root->left, env, STDIN_FILENO);
            else
                execute_command(root->left, env);
                
            exit(EXIT_FAILURE);  // Should not reach here
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
           // free_tree_exe(root);
            return 1;
        } 
        else if (pid_right == 0) 
        {
            // Child process for right command
            close(pipefd[1]);  // Close unused pipe write end
            
            if (input_fd != STDIN_FILENO)
                close(input_fd);
                
            // Redirect stdin to pipe read end
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);  // Close after dup2
            
            // Execute right subtree
            if (root->right->type == PIPE)
                exec_pipe(root->right, env, STDIN_FILENO);
            else
                execute_command(root->right, env);
                
            exit(EXIT_FAILURE);  // Should not reach here
        }
        
        // Parent process cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        if (input_fd != STDIN_FILENO)
            close(input_fd);
            
        // Wait for all child processes to complete
        waitpid(pid_left, &status, 0);
        waitpid(pid_right, &status, 0);
        
        // Clean up and return
        //free_tree_exe(root);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return 1;
    }
    // else 
    // {
    //     // Direct command execution (no pipe)
    //     if (input_fd != STDIN_FILENO)
    //     {
    //         dup2(input_fd, STDIN_FILENO);
    //         close(input_fd);
    //     }
        
    //     execute_command(root, env);
    //     //free_tree_exe(root);
    //     exit(EXIT_FAILURE);  // Should not reach here
    // }
    
    return 1;
}


void execute_command(t_tree *root, char **env) 
{
	if (!root || !root->data || !root->data[0]) 
	{
		fprintf(stderr, "Error: Empty command node\n");
		exit(EXIT_FAILURE);
	}

	if (is_builtin(root->data[0]) == 0) 
	{
		execute_builtin(root, env);
		exit(EXIT_SUCCESS);
	}

	char *binary_path = get_binary_file_path(root, env);
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

int execution(t_tree *root, char **env)
{
    int status = 0;
    pid_t pid;
    
    if (!root)
        return 1;
    if (root->type == PIPE)
    {
        printf("[execution function] Processing pipe and multiple commands\n");
        
        // Create a copy of the tree for execution
        t_tree *exec_tree = root;
        
        // Execute the pipe tree
        status = exec_pipe(exec_tree, env, STDIN_FILENO);
        
        // Free the original tree when done
       // free_tree_exe(root);
        
        return status;
    }
    
    if (is_builtin(root->data[0]) == 0)
    {
        printf("Processing built-in-command and no pipe, not binary command, just function\n");
        execute_builtin(root, env);
        free_tree_exe(root);
        return 0;
    }
    
    // Handle regular command (no pipe)
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free_tree_exe(root);
        return 1;
    }
    else if (pid == 0)
    {
        // Child process executes the command
        execute_command(root, env);
        // This shouldn't be reached, but just in case
        exit(EXIT_FAILURE);
    }
    
    // Parent waits for child to complete
    waitpid(pid, &status, 0);
    
    // Free the command tree in the parent process
   // free_tree_exe(root);
    
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return 1;
}




