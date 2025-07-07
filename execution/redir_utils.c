/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:54:10 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 14:25:11 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

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

static int expand_and_reopen_fd(int fd, t_env **env_list)
{
	int		fd_in;
	int		fd_out;
	char	*line;
	char	tmp_path[64];

	ft_strlcpy(tmp_path,  "/tmp/heredocXXXXXX_expand", sizeof(tmp_path));	
	fd_in = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	fd_out = open(tmp_path, O_RDONLY, 0644);
	unlink(tmp_path);
	if (fd_in == -1 || fd_out == -1)
		return (close(fd_out), close(fd_in), -1);
	while ((line = get_next_line(fd)))
	{
		if (expand_herdoc(&line, *env_list) == R_FAIL)
		{
			free(line);
			return (close(fd_out), close(fd_in), -1);
		}
		ft_putendl_fd(line, fd_in);
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
		fd =  expand_and_reopen_fd( fd, env_list);
	if (dup2(fd, target) == -1)
	{
		perror("minishell: dup2 failed");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	apply_single_redirection(t_red *redir, t_env **env_list)
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

int	apply_redirections(t_red *redir, t_env **env_list)
{
	while (redir)
	{
		if (redir->is_ambiguous == 1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redir->data, STDERR_FILENO);
			ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
			return (1);
		}
		if (apply_single_redirection(redir, env_list) == 1)
			return (1);
		redir = redir->next;
	}
	return (0);
}
