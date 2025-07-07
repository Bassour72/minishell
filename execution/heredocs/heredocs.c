/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:23:03 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 21:05:25 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

void	setup_heredoc_handler(int sig)
{
	(void)sig;
	g_exit_status = 1;
	write(STDOUT_FILENO, "\n", 1);
}

static void	child_heredoc_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(1);
}

static int	child_read_heredoc(int fd, const char *limiter, t_env **env_list)
{
	char	*line;

	signal(SIGINT, child_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);

	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	exit(0);
}

int	write_heredoc(int fd, const char *limiter, t_env **env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(fd);
		return (1);
	}
	else if (pid == 0)
		child_read_heredoc(fd, limiter, env_list);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 1)
		return (1);
	return (0);
}

int	create_heredoc(t_red *redir, t_env **env_list)
{
	int		fd;
	char	*tmp_path = "/tmp/heredocXXXXXX";

	fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	redir->out_fd = open(tmp_path, O_RDONLY, 0644);
	unlink(tmp_path);
	if (fd == -1 || redir->out_fd == -1)
	{
		if (fd != -1) close(fd);
		if (redir->out_fd != -1) close(redir->out_fd);
		perror("heredoc open failed");
		exit(EXIT_FAILURE);
	}
	if (write_heredoc(fd, redir->data, env_list))
	{
		close(fd);
		close(redir->out_fd);
		redir->out_fd = -1;
		return (1);
	}
	close(fd);
	return (0);
}

int	prepare_heredocs(t_tree *root, t_env **env_list)
{
	t_red *redir;

	if (!root)
		return (0);
	redir = root->redirections;
	if (expand_herdoc_delimiter(redir, *env_list) == R_FAIL)
		return (1);
	while (redir)
	{
		if (redir->type == HER_DOC)
		{
			if (create_heredoc(redir, env_list))
				return (1);
			if (g_exit_status == 1)
				return (1);
		}
		if (g_exit_status == 1)
			return (1);
		redir = redir->next;
	}
	if (prepare_heredocs(root->left, env_list))
		return (1);
	if (prepare_heredocs(root->right, env_list))
		return (1);
	return (0);
}
