/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:33:14 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 21:56:30 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

static int	wait_for_children(pid_t pid_left, pid_t pid_right)
{
	int	status;
	int	exit_code;

	status = 0;
	exit_code = 1;
	if (pid_right > 0)
	{
		waitpid(pid_right, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
	}
	while (wait(NULL) != -1)
		;
	return (exit_code);
}

static pid_t	fork_left_process(t_tree *root, t_env **env_list, \
int pipe[2], bool is_child)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		if (is_child)
			check_non_interactive_exit(root, env_list, -1, true);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe[0]);
		dup2(pipe[1], STDOUT_FILENO);
		close(pipe[1]);
		status = exec_tree(root->left, env_list, 1, true);
		check_non_interactive_exit(root, env_list, status, true);
	}
	return (pid);
}

static pid_t	fork_right_process(t_tree *root, t_env **env_list, \
int pipe[2], bool is_child)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		if (is_child)
			check_non_interactive_exit(root, env_list, -1, true);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe[1]);
		dup2(pipe[0], STDIN_FILENO);
		close(pipe[0]);
		status = exec_tree(root->right, env_list, 1, true);
		check_non_interactive_exit(root, env_list, status, true);
	}
	return (pid);
}

static	void	close_parent_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	exec_pipe(t_tree *root, t_env **env_list, bool is_child)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (!root || root->type != PIPE)
		return (1);
	if (create_pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid_left = fork_left_process(root, env_list, pipefd, is_child);
	if (pid_left < 0)
		return (close_pipe_fds(pipefd), 1);
	pid_right = fork_right_process(root, env_list, pipefd, is_child);
	if (pid_right < 0)
	{
		close_pipe_fds(pipefd);
		if (pid_left > 0)
			waitpid(pid_left, NULL, 0);
		return (1);
	}
	close_pipe_fds(pipefd);
	return (wait_for_children(pid_left, pid_right));
}
