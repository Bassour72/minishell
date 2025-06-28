// #include "../include/execution.h"
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
			sleep(6);
			fprintf(stderr,"@@@@@@@@@@@@@[here for debugg the minishell for check if has leaks file descriptor]input_fd[%d]", input_fd);
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
			sleep(6);
			fprintf(stderr,"@@@@@@@@@@@@@[here for debugg the minishell for check if has leaks file descriptor]input_fd[%d]", input_fd);
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
			sleep(6);
			fprintf(stderr,"@@@@@@@@@@@@@[here for debugg the minishell for check if has leaks file descriptor]input_fd[%d]", input_fd);
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
		sleep(6);
		fprintf(stderr,"@@@@@@@@@@@@@[here for debugg the minishell for check if has leaks file descriptor]input_fd[%d]", input_fd);
		close(input_fd);
	}
	dup2(0, STDIN_FILENO);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFSIGNALED(status))
	{
		int s = WTERMSIG(status);
		printf("if (WIFSIGNALED(status))[%d] \n ",s);
	}
	if (WIFEXITED(status)) 
	{
		printf("if (WIFEXITED(status))[%d] \n ",status);
		int s = WTERMSIG(status);
		return WEXITSTATUS(status);
	}
	return (1);
}