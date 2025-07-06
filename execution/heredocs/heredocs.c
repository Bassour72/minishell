/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:23:03 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/06 23:31:28 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

void	setup_heredoc_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}

static int	handle_null_line(int stdin_backup, const char *limiter)
{
	if (g_exit_status == 130)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		return (130);
	}
	return (0);
}

static int	read_and_write_heredoc(int fd, const char *limiter, \
t_env **env_list, int stdin_backup)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_exit_status == 130)
			{
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
				return (1);
			}
			break ;
		}
		if (ft_strcmp(line, limiter) == 0)
			break ;
		if (expand_herdoc(&line, *env_list) == R_FAIL)
			return (1);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

int	write_heredoc(int fd, const char *limiter, t_env **env_list)
{
	int	stdin_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	g_exit_status = 0;
	signal(SIGINT, setup_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	status = read_and_write_heredoc(fd, limiter, env_list, stdin_backup);
	if (status == 1)
		return (status);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	return (status);
}

int	create_heredoc(t_red *redir, t_env **env_list)
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
