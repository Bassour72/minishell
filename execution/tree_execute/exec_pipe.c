#include "../../include/execution.h"

static int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static pid_t	fork_left_process(t_tree *root, char **env, t_env **env_list,
		int input_fd, int pipe_out)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		dup2(pipe_out, STDOUT_FILENO);
		close(pipe_out);
		exit(exec_tree(root->left, env, env_list, STDIN_FILENO, 1));
	}
	return (pid);
}

static pid_t	fork_right_process(t_tree *root, char **env, t_env **env_list,
		int input_fd, int pipe_in)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_in + 1);
		dup2(pipe_in, STDIN_FILENO);
		close(pipe_in);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		exit(exec_tree(root->right, env, env_list, STDIN_FILENO, 1));
	}
	return (pid);
}

static void	close_parent_fds(int pipefd[2], int input_fd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}

static int	wait_for_children(pid_t pid_left, pid_t pid_right)
{
	int	status;

	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_pipe(t_tree *root, char **env, int input_fd, t_env **env_list)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (!root || root->type != PIPE)
		return (1);
	if (create_pipe(pipefd))
		return (1);
	pid_left = fork_left_process(root, env, env_list, input_fd, pipefd[1]);
	if (pid_left < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	pid_right = fork_right_process(root, env, env_list, input_fd, pipefd[0]);
	if (pid_right < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		return (1);
	}
	close_parent_fds(pipefd, input_fd);
	return (wait_for_children(pid_left, pid_right));
}
