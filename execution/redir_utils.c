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

static int	dup_redir_fd(t_red *redir, int fd)
{
	int	target;
	target = STDOUT_FILENO;
	if (redir->type == RED_INPUT || redir->type == HER_DOC)
		target = STDIN_FILENO;
	if (dup2(fd, target) == -1)
	{
		perror("minishell: dup2 failed");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	apply_single_redirection(t_red *redir)
{
	int	fd;

	fd = open_redir_fd(redir);
	if (fd == -1)
	{
		perror("minishell: redirection open failed");
		return (1);
	}
	return (dup_redir_fd(redir, fd));
}

int apply_redirections(t_red *redir, t_env **env_list)
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
		if (apply_single_redirection(redir) == 1)
			return (1);
		redir = redir->next;
	}
	return (0);
}
