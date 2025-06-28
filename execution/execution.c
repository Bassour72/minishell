#include "../include/execution.h"

//todo remove this 
//fixme 
//todo you don't need to share **env with all fucntion 

void setup_exit_handler(int sig)
{
	(void)sig;
    g_exit_status = 130;
	exit(130);
	// close(STDIN_FILENO);
	// write(STDOUT_FILENO, "\n", 1);
}

int wait_child_status(int input_fd, pid_t pid, t_env **env_list)
{
	int status;
	int sig;

	if (input_fd != STDIN_FILENO)
		close(input_fd);

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) //true if child exited normally.
	{
		int s = WTERMSIG(status); 
		sig = WEXITSTATUS(status); // — gives the exit code. // Gets exit code (if WIFEXITED is true)
		return (sig);
	}
	if (WIFSIGNALED(status)) // true if child was terminated by a signal.
	{
		 sig = WTERMSIG(status); // Which signal caused termination
		return (sig + 128);
	}
	return (1);
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
		//todo should check if fork return - 1; 
		// todo free all resources then exit
		if (pid == 0) 
		{
			// signal(SIGINT, SIG_DFL);             // Let Ctrl+C work as expected
			// signal(SIGQUIT, SIG_DFL);
			// execute the inner subtree in child process
			if (expand_redir(root->redirections, *env_list) == R_FAIL)
				return (1);
			if (apply_redirections(root->redirections, env_list)==1)
			{
				//free_tree(root);
					exit(1);
				return (1);
			}
			exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
			exit(EXIT_SUCCESS);
		} 
		else 
		{
			int status;
			waitpid(pid, &status, 0);
			return WEXITSTATUS(status);
		}
	}
	if (root->type == BLOCK) 
	{
		if (root->data)
		{
			//todo
			if (expand(&root->data, *env_list) == R_FAIL)
				return(1);
			// if (wildcard(&root->data) == R_FAIL)
			// 	return(1);
		}
		//todo check here the expand fucntion return int 
		if (in_subshell)
		{
			
			if (input_fd != STDIN_FILENO)
			{
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
			}
			run_command(root, env, env_list); // or execute_external_command
			exit(EXIT_FAILURE); // if it fails

		/* 	//todo Always fork for commands in subshells (e.g., pipelines)
			pid_t pid = fork();
			//todo should check if fork return - 1; 
		// todo free all resources then exit
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
				printf("here int the exec_tree block in subshell [%d] fucntion for check if found leaks \n", status);
				//sleep(6);
				if (WIFEXITED(status))
				{
					return WEXITSTATUS(status);
				}
				return 1;
			} */
		}
		else
		{
			//todo Not in subshell: execute built-ins directly, fork for external commands
			if (root->data != NULL &&  is_builtin(root->data[0]) == 0)
			{
				if (expand_redir(root->redirections, *env_list) == R_FAIL)
					return (1);
				int saved_stdin = dup(STDIN_FILENO);
				int saved_stdout = dup(STDOUT_FILENO);
				if (apply_redirections(root->redirections, env_list) == 1)
					return (1);
				int built_in_status = execute_builtin(root, env, env_list);
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				return (built_in_status);

			}
			pid_t pid = fork();
			if (pid < 0) 
			{
				perror("fork");
				return 1;
			}
			if (pid == 0) 
			{
				signal(SIGINT, SIG_DFL);             // Let Ctrl+C work as expected
				signal(SIGQUIT, SIG_DFL);
				if (expand_redir(root->redirections, *env_list) == R_FAIL)
					return (1); //todo here you in the child process should use exit not return because error
				if (apply_redirections(root->redirections, env_list) == 1)
				{
					//free_tree(root);
					exit(1);
					return 1; // 
				}
				if (input_fd != STDIN_FILENO)
				{
					printf("FOR CHECK THE FILE DIRCRIPTE HERE ONE FUCK OFF \n ");
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
				}
				execute_external_command(root, env_list);
			}
			else if (pid > 0)
			{
				return (wait_child_status(input_fd, pid, env_list));
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
	dup2(0, STDIN_FILENO);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFSIGNALED(status))
	{
		int s = WTERMSIG(status);
	}
	if (WIFEXITED(status)) 
	{
		int s = WTERMSIG(status);
		return WEXITSTATUS(status);
	}
	return (1);
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
	int status;

	if (!root) 
		return (1);
	if (is_forkred(root))
    	propagate_fork_flag(root, 1);
	else
    	 propagate_fork_flag(root, 0);
	status = prepare_heredocs(root, env_list);
	if (status != 0)
	{
		if (g_exit_status == 130)
			status = g_exit_status;
		return (status);
	}
	status = exec_tree(root, env, env_list, STDIN_FILENO, 0);
	g_exit_status = 0;
	return (status);
}
