#include "../include/execution.h"

int show_warning_heredoc(int boolean, const char *limiter)
{
	if (boolean)
	{
		write(2, "minishell: warning: here-document ", 35);
		write(2, "delimited by end-of-file (wanted `", 35);
		write(2, limiter, ft_strlen(limiter));
		write(2, "')\n", 3);
	}
	return (0);
}

void setup_heredoc_handler(int sig)
{
	(void)sig;
    g_exit_status = 130;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}

int write_heredoc(int fd, const char *limiter, t_env **env_list)
{
    char *line;
	int stdin_backup;
	stdin_backup = dup(STDIN_FILENO);
   	g_exit_status = 0;
	signal(SIGINT, setup_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
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
			show_warning_heredoc(1, limiter);
			break;
		}
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
		if (expand_herdoc(&line, *env_list) == R_FAIL)
			return (1);// check the return value on fail ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
	close(stdin_backup);
	return (0);
}

int create_heredoc(t_red *redir, t_env **env_list)
{
	int fd;
	char tmp_path[] = "/tmp/heredocXXXXXX";

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



int prepare_heredocs(t_tree *root, t_env **env_list)
{

	int	return_exit;
	if (!root)
		return (0);
	t_red *redir = root->redirections;
	if (expand_herdoc_delimiter(redir, *env_list) == R_FAIL)
		return (0);
	if (expand_redir(redir, *env_list) == R_FAIL)
		return (1);
	while (redir)
	{
		if (redir->type == HER_DOC)
		{
			return_exit = create_heredoc(redir, env_list);
			if (return_exit != 0)
				return (return_exit);
		}
		redir = redir->next;
	}
	if (prepare_heredocs(root->left, env_list) == 1)
		return (1);
	if (prepare_heredocs(root->right, env_list) == 1)
		return (1);
	return (0);
}

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
