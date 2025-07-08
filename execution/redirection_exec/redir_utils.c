/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:54:10 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 18:03:28 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

static int	open_redir_fd(t_red *redir)
{
	if (redir->type == RED_INPUT)
		return (open(redir->data, O_RDONLY));
	else if (redir->type == RED_TRUNK)
		return (open(redir->data, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (redir->type == RED_APPEND)
		return (open(redir->data, O_WRONLY | O_CREAT | O_APPEND, 0644));
	else if (redir->type == HER_DOC)
		return (redir->out_fd);
	return (-1);
}

static int	create_temp_heredoc_fds(int *fd_in, int *fd_out, char *tmp_path)
{
	ft_strlcpy(tmp_path, "/tmp/heredocXXXXXX_expand", 64);
	*fd_in = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	*fd_out = open(tmp_path, O_RDONLY, 0644);
	unlink(tmp_path);
	if (*fd_in == -1 || *fd_out == -1)
		return (close(*fd_out), close(*fd_in), -1);
	return (0);
}

static int	expand_and_reopen_fd(int fd, t_env **env_list)
{
	int		fd_in;
	int		fd_out;
	char	*line;
	char	tmp_path[64];

	if (create_temp_heredoc_fds(&fd_in, &fd_out, tmp_path) == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (expand_herdoc(&line, *env_list) == R_FAIL)
		{
			free(line);
			return (close(fd_out), close(fd_in), -1);
		}
		ft_putstr_fd(line, fd_in);
		free(line);
	}
	close(fd);
	close(fd_in);
	return (fd_out);
}

static int	dup_redir_fd(t_red *redir, int fd, t_env **env_list)
{
	int	target;

	target = STDOUT_FILENO;
	if (redir->type == RED_INPUT || redir->type == HER_DOC)
		target = STDIN_FILENO;
	if (redir->type == HER_DOC)
		fd = expand_and_reopen_fd(fd, env_list);
	if (dup2(fd, target) == -1)
	{
		perror("minishell: dup2 failed");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_single_redirection(t_red *redir, t_env **env_list)
{
	int	fd;

	fd = open_redir_fd(redir);
	if (fd == -1)
	{
		perror("minishell:");
		return (1);
	}
	return (dup_redir_fd(redir, fd, env_list));
}
