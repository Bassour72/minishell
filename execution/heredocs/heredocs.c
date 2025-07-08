/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:23:03 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 14:51:38 by massrayb         ###   ########.fr       */
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

static int	child_read_heredoc(int fd, const char *limiter)
{
	char	*line;

	signal(SIGINT, child_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	exit(0);
}

int	write_heredoc(int fd, const char *limiter)
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
		child_read_heredoc(fd, limiter);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 1)
		return (1);
	return (0);
}

int	create_heredoc(t_red *redir)
{
	int		fd;
	char	*tmp_path;

	tmp_path = "/tmp/heredocXXXXXX";
	fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	redir->out_fd = open(tmp_path, O_RDONLY, 0644);
	unlink(tmp_path);
	if (fd == -1 || redir->out_fd == -1)
	{
		if (fd != -1)
			close(fd);
		if (redir->out_fd != -1)
			close(redir->out_fd);
		perror("heredoc open failed");
		exit(EXIT_FAILURE);
	}
	if (write_heredoc(fd, redir->data))
	{
		close(fd);
		close(redir->out_fd);
		redir->out_fd = -1;
		return (1);
	}
	close(fd);
	return (0);
}
