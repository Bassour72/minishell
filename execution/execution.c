#include "../include/execution.h"



//todo remove this 
//fixme 


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
		return cd_change_working_directory(root);
	if (strcmp(root->data[0], "pwd") == 0)
		return pwd_print_working_directory(root);
	if (strcmp(root->data[0], "env") == 0)
		return env_environment(root, env);
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


void execute_external_command(t_tree *root, char **env) 
{
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

void run_command(t_tree *root, char **env, t_env **env_list) 
{
	if (!root || !root->data || !root->data[0]) 
	{
		fprintf(stderr, "Error: Empty command node\n");
		free_tree_exe(root);
		exit(EXIT_FAILURE);
	}
	apply_redirections(root->redirections);
	if (is_builtin(root->data[0]) == 0) 
	{
		int status = execute_builtin(root, env, env_list);
		free_tree_exe(root);
		exit(status);
	}
	execute_external_command(root, env);
	exit(EXIT_FAILURE); //todo Unreachable
}


int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell) 
{
	if (!root) 
	{
		return 1;
	}

	if (root->type == BLOCK) 
	{
		
	   expand(*env_list, root);
		//todo check here the expand fucntion return int 
		if (in_subshell)
		{
			printf("here 2\n\n\n");
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
			apply_redirections(root->redirections);
			printf("here 1\n\n\n");
			if (is_builtin(root->data[0]) == 0)
			{
				return execute_builtin(root, env, env_list);
			}
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
				execute_external_command(root, env);
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

	if (WIFEXITED(status)) 
	{
		return WEXITSTATUS(status);
	}
	return 1;
}


int execution(t_tree *root, char **env, t_env **env_list)
{
	if (!root) 
		return 1;
	prepare_heredocs(root);
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