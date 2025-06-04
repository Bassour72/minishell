#include "../include/execution.h"

/*



if (WIFSIGNALED(status)) {
    int sig = WTERMSIG(status);
    if (sig == SIGINT)
        g_exit_status = 130;
    else if (sig == SIGQUIT)
        g_exit_status = 131;
}

*/

//todo remove this 
//fixme 
//todo you don't need to share **env with all fucntion 

int is_builtin(char *command) 
{
	if (strcmp(command, "echo") == 0 || strcmp(command, "ECHO") == 0)
		return 0;
	if (strcmp(command, "cd") == 0)
		return 0;
	if (strcmp(command, "env") == 0)
		return 0;
	if (strcmp(command, "exit") == 0)
		return 0;
	if (strcmp(command, "export") == 0)
		return 0;
	if (strcmp(command, "pwd") == 0)
		return 0;
	if (strcmp(command, "unset") == 0)
		return 0;
	return 1;
}


int execute_builtin(t_tree *root, char **env, t_env **env_list)
{
	if (strcmp(root->data[0], "echo") == 0)
		return builtin_echo(root);
	if (strcmp(root->data[0], "cd") == 0)
		return cd_change_working_directory(root, env_list);
	if (strcmp(root->data[0], "pwd") == 0)
		return pwd_print_working_directory(root, env_list);
	if (strcmp(root->data[0], "env") == 0)
		return env_environment(root, env, *env_list);
	if (strcmp(root->data[0], "exit") == 0)
	{
		exit_exe(root);
		return 0;
	}
	if (strcmp(root->data[0], "export") == 0)
		return export_command_builtin(root, env_list);
	if (strcmp(root->data[0], "unset") == 0)
		return builtin_unset_environment(root, env_list, env);
	return 1;
}
int env_list_len(t_env *env_list)
{
    int len = 0;
    while (env_list)
    {
        len++;
        env_list = env_list->next;
    }
    return len;
}

char *create_env_string(t_env *node)
{
	size_t key_len;
    size_t value_len;
    size_t total_len;
	char *env_string;

    key_len =  0;
	if (node->key)
		key_len =  ft_strlen(node->key);

	value_len = 0;
	if (node->value)
    	value_len = ft_strlen(node->value);

    total_len = key_len + value_len + 2;

    env_string = malloc(total_len);
    if (!env_string)
        return (NULL);

    if (node->key)
        strcpy(env_string, node->key);
    else
        env_string[0] = '\0';
    strcat(env_string, "=");
    if (node->value)
        strcat(env_string, node->value);

    return env_string;
}

char **gen_new_env(t_env *env_list)
{
    int len;
	int i;
	char **new_env;
    t_env *tmp;
	len = env_list_len(env_list);
	tmp = env_list;
    new_env = malloc(sizeof(char *) * (len + 1));
    if (!new_env)
        return (NULL);

    tmp = env_list;
    i = 0;
    while (tmp)
    {
        new_env[i] = create_env_string(tmp);
        if (!new_env[i])
        {
            while (i > 0)
                free(new_env[--i]);
            free(new_env);
            return (NULL);
        }
        i++;
        tmp = tmp->next;
    }
    new_env[i] = NULL;
    return (new_env);
}

void execute_external_command(t_tree *root, char **env, t_env **env_list) 
{
	// sleep(2);
	printf("inside the execute external command \n ");
	char *binary_path = get_binary_file_path(root, env,env_list);
	//sleep(2);
	printf("check if the get_binary_file_path return success value*************[%s] \n ", binary_path);
	if (!binary_path) 
	{
		fprintf(stderr, "Error: Command not found: %s\n", root->data[0]);
		free_tree_exe(root);
		exit(EXIT_FAILURE);
	}
	char **new_env = gen_new_env(*env_list);
	// signal(SIGINT, SIG_DFL);
	// signal(SIGQUIT, SIG_DFL);

	execve(binary_path, root->data, new_env);
	//sleep(2);
	printf("here check if execve return error\n ");
	perror("execve");
	free(binary_path);
	free_tree_exe(root);
	exit(EXIT_FAILURE);
}

void run_command(t_tree *root, char **env, t_env **env_list) 
{
	if (!root || !root->data || !root->data[0]) 
	{
		fprintf(stderr, "Error: Empty command node \n");
		free_tree_exe(root);
		exit(EXIT_FAILURE);
	}
	apply_redirections(root->redirections);
	//sleep(2);
	printf("Run command for check the redirections \n ");
	/*
		free_tree_exe(root);
		exit(EXIT_FAILURE);
	*/
	if (is_builtin(root->data[0]) == 0) 
	{
		int status = execute_builtin(root, env, env_list);
		free_tree_exe(root);
		exit(status);
	}
	execute_external_command(root, env, env_list);
	exit(EXIT_FAILURE); //todo Unreachable
}

int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell) 
{
	if (!root) 
	{
		return 1;
	}
	if (root->data == NULL && root->type == BLOCK && root->left)
	{

		// this is a subshell (e.g., due to parentheses)
		pid_t pid = fork();
		if (pid == 0) 
		{
			// execute the inner subtree in child process
			apply_redirections(root->redirections);
			exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
			exit(EXIT_SUCCESS);
		} else 
		{
			// wait for child
			int status;
			waitpid(pid, &status, 0);
			return WEXITSTATUS(status);
		}
	}
	if (root->type == BLOCK) 
	{
		//sleep(2);
		printf("here in exec tree for one command \n ");
		if (root->data)
		{
			char **old_args = root->data;
			if (expand(&root->data, old_args, *env_list) == R_FAIL)
				return(1);
			
		}
		//todo check here the expand fucntion return int 
		if (in_subshell)
		{
			printf("int subshell here 2\n\n\n");
			//todo Always fork for commands in subshells (e.g., pipelines)
			pid_t pid = fork();
			if (pid < 0)
			{
				perror("fork");
				return 1;
			}
			if (pid == 0) 
			{
				if (input_fd != STDIN_FILENO)
				{
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
				}
				run_command(root, env, env_list);
				exit(EXIT_FAILURE); //todo Unreachable
			}
			if (pid > 0)
			{
				int status;
				if (input_fd != STDIN_FILENO)
				{
					close(input_fd);
				}
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
				{
					return WEXITSTATUS(status);
				}
				return 1;
			}
		} 
		else
		{
			//todo Not in subshell: execute built-ins directly, fork for external commands
			//sleep(2);
			printf("in the exec tree for check why the program exit and the entered is valide command\n ");
			
			printf("here 1\n\n\n");
			if (root->data != NULL &&  is_builtin(root->data[0]) == 0)
			{
				//sleep(2);
				printf("does built-in fucntion \n ");
				int saved_stdin = dup(STDIN_FILENO);
				int saved_stdout = dup(STDOUT_FILENO);
				apply_redirections(root->redirections);
				int built_in_status = execute_builtin(root, env, env_list);
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				return (built_in_status);

			}
			pid_t pid = fork();
			
			//sleep(2);
			printf("for pid-----------------------[%d] \n ", pid);
			if (pid < 0) 
			{
				perror("fork");
				return 1;
			}
			if (pid == 0) 
			{
				apply_redirections(root->redirections);
				if (input_fd != STDIN_FILENO)
				{
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
				}
				// sleep(2);
				 printf("open the execute external command \n ");
				execute_external_command(root, env, env_list);
				// sleep(2);
				printf("here for  check why it exit and should not exit\n ");
				//exit(EXIT_FAILURE); //todo Unreachable
			}
			if (pid > 0) 
			{
				int status;
				if (input_fd != STDIN_FILENO)
				{
					close(input_fd);
				}
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
				{
					return WEXITSTATUS(status);
				}
				return 1;
			}
		}
	}
	if (root->type == PIPE)
	{
		return exec_pipe(root, env, input_fd, env_list);
	}
	if (root->type == OP_AND)
	{
		int left_status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
		if (left_status == 0)
		{
			return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
		}
		return left_status;
	}
	if (root->type == OP_OR)
	{
		int left_status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
		if (left_status != 0) 
		{
			return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
		}
		return left_status;
	}
	return 1;
}

int exec_pipe(t_tree *root, char **env, int input_fd, t_env **env_list)
{
	int pipefd[2];
	pid_t pid_left, pid_right;
	int status = 0;
	if (!root || root->type != PIPE) 
	{
		return 1;
	}

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return 1;
	}
	pid_left = fork();
	if (pid_left < 0) 
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return 1;
	}
	if (pid_left == 0) 
	{
		//todo Left child: set input and output
		if (input_fd != STDIN_FILENO) 
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(exec_tree(root->left, env, env_list, STDIN_FILENO, 1)); //todo in_subshell=1
	   //todo exit(EXIT_FAILURE);
	}

	pid_right = fork();
	if (pid_right < 0) 
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		if (input_fd != STDIN_FILENO) 
		{
			close(input_fd);
		}
		return 1;
	}
	
	if (pid_right == 0) 
	{
		//todo Right child: set input
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (input_fd != STDIN_FILENO) 
		{
			close(input_fd);
		}
		exit(exec_tree(root->right, env, env_list, STDIN_FILENO, 1)); //todo in_subshell=1
	   //todo exit(EXIT_FAILURE);
	}

	//todo Parent: clean up and wait
	close(pipefd[0]);
	close(pipefd[1]);
	if (input_fd != STDIN_FILENO) 
	{
		close(input_fd);
	}

	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);

	if (WIFSIGNALED(status))
	{
		int s = WTERMSIG(status);
	}
	if (WIFEXITED(status)) 
	{
		
		return WEXITSTATUS(status);
	}
	return 1;
}

int is_forkred(t_tree *root)
{
    if (root == NULL)
        return 0;
    if (root->type == PIPE)
        return 1;
    if (is_forkred(root->left))
        return 1;
    if (is_forkred(root->right))
        return 1;
    return 0;
}
void propagate_fork_flag(t_tree *root, int is_forked)
{
    if (!root)
        return;
    root->is_forked = is_forked;
    if (root->type == PIPE)
    {
        propagate_fork_flag(root->left, 1);
        propagate_fork_flag(root->right, 1);
    }
    else
    {
        propagate_fork_flag(root->left, is_forked);
        propagate_fork_flag(root->right, is_forked);
    }
}

int execution(t_tree *root, char **env, t_env **env_list)
{
	if (!root) 
		return 1;
	if (is_forkred(root))
    	propagate_fork_flag(root, 1);
	else
	{
    	 propagate_fork_flag(root, 0);
		// char *binary_path = get_binary_file_path(root, env,env_list);
		// update_last_executed_command(env_list, binary_path);
	}
	prepare_heredocs(root); //todo check if the it command and has heredoc
	//sleep(2);
	printf("exection fucntion \n ");
	int status = exec_tree(root, env, env_list, STDIN_FILENO, 0);
	free_tree_exe(root);
	return status;
}


void free_tree_exe(t_tree *root) 
{
	int i;
	if (!root)
		return;
	free_tree_exe(root->left);
	free_tree_exe(root->right);
	if (root->data) 
	{
		i = 0;
		while (root->data[i] != NULL) 
		{
			free(root->data[i]);
			i++;
		}
		free(root->data);
	}
	free(root);
}