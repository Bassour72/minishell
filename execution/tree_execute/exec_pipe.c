#include "../../include/execution.h"

#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
/*

for (int fd = 3; fd < 1024; fd++)
	if (is_fd_open(fd))
		dprintf(2, "FD %d is still open\n", fd);


*/
int is_fd_open(int fd)
{
    struct winsize ws;
    if (ioctl(fd, TIOCGWINSZ, &ws) == -1)
    {
        if (errno == EBADF)
            return 0; // FD is not open
    }
    return 1; // FD is open (or at least valid for ioctl)
}

static void	close_all_fds_(void)
{
	int fd;

	fd = 0;
	while (fd <= 40)
    {
		close(fd);
		// if (!isatty(fd))
        // {
		// 	// if (is_fd_open(fd))
		// 	// {
		// 	// 	printf("hekkkkkkkkkkkkkkkkkkkkkk======================================*****************************************\n\n");
		// 	// 	sleep(1);
		// 	// }
        // }
		 ++fd;
	}
}

static int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

static pid_t	fork_left_process(t_tree *root, t_env **env_list, int pipe[2])
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
		close(pipe[0]);
		dup2(pipe[1], STDOUT_FILENO);
		close(pipe[1]);
		int k =  exec_tree(root->left, env_list, 1);
		fprintf(stderr, "========================left:%d\n", getpid());
		close_all_fds_();

		
		exit(k);
	}
	// else
	// {
	// 	close(pipe[1]);  // Parent must close write-end
	// }
	return (pid);
}

static pid_t	fork_right_process(t_tree *root, t_env **env_list, int pipe[2])
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
		close(pipe[1]);
		dup2(pipe[0], STDIN_FILENO);
		close(pipe[0]);
		//close_all_fds_();
		int k = exec_tree(root->right, env_list, 1);
				fprintf(stderr,"========================tigth:%d\n", getpid());

		close_all_fds_();
		exit(k);
	}
	// else
	// {
	// 	close(pipe[0]);  // Parent must close write-end
	// }
	return (pid);
}

static void	close_parent_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
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

// int	exec_pipe(t_tree *root, t_env **env_list)
// {
// 	int		pipefd[2];
// 	pid_t	pid_left;
// 	pid_t	pid_right;

// 	if (!root || root->type != PIPE)
// 		return (1);
// 	if (create_pipe(pipefd))
// 		return (1);
// 	pid_left = fork_left_process(root, env_list,  pipefd);
// 	// close(pipefd[0]);
// 	if (pid_left < 0)
// 	{
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		return (1);
// 	}
// 	pid_right = fork_right_process(root,  env_list, pipefd);
// 	// close(pipefd[1]);
// 	if (pid_right < 0)
// 	{
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		return (1);
// 	}
// 	close_parent_fds(pipefd);
// 	return (wait_for_children(pid_left, pid_right));
// }
void	close_pipe_fds(int pipefd[2])
{
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
}

int	exec_pipe(t_tree *root, t_env **env_list)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (!root || root->type != PIPE)
		return (1);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);

	pid_left = fork_left_process(root, env_list, pipefd);
	if (pid_left < 0)
		return (close_pipe_fds(pipefd), 1);

	pid_right = fork_right_process(root, env_list, pipefd);
	if (pid_right < 0)
	{
		waitpid(pid_left, NULL, 0);
		return (close_pipe_fds(pipefd), 1);
	}

	// Parent must close both ends of the pipe
	close_pipe_fds(pipefd);

	return (wait_for_children(pid_left, pid_right));
}
