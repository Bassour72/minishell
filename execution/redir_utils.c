#include "../include/execution.h"



//todo remove this 
//fixme 

int show_warning_heredoc(int boolean, const char *limiter)
{
	if (boolean)
	{
		write(2, "minishell: warning: here-document ", 35);
		write(2, "delimited by end-of-file (wanted `", 35);
		write(2, limiter, ft_strlen(limiter));
		write(2, "')\n", 3);
	}
	return 0;
}



void setup_heredoc_handler(int sig)
{
	(void)sig;
	//dup2(STDERR_FILENO, STDOUT_FILENO);
    g_exit_status = 130;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	// dup2(2, 0);
	 printf("here 1000000000000000000000000\n");
}


int write_heredoc(int fd, const char *limiter, t_env **env_list)
{
    char *line;

   	g_exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, setup_heredoc_handler);
   while (1)
   {
        line = readline("> ");
		if (!line)
		{
			if (g_exit_status == 130)
				return 1; // Interrupted
			show_warning_heredoc(1, limiter);
			break;
		}

        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
	return 0;
}


int create_heredoc(t_red *redir, t_env **env_list)
{
	char tmp_path[] = "/tmp/heredocXXXXXX";
	int fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		return 1;  // heredoc interrupted
	}

	close(fd);
	return 0;
}

int prepare_heredocs(t_tree *root, t_env **env_list)
{
	if (!root)
		return 0;
	t_red *redir = root->redirections;

	while (redir)
	{
		if (redir->type == HER_DOC)
		{
			if (create_heredoc(redir, env_list) == 1)
				return 1; // Interrupted, stop early
		}
		redir = redir->next;
	}
	if (prepare_heredocs(root->left, env_list) == 1)
		return 1;
	if (prepare_heredocs(root->right, env_list) == 1)
		return 1;
	return 0;
}


void apply_redirections(t_red *redir, t_env **env_list) 
{
	int fd;
	
	if (redir == NULL)
		return ;
	while (redir) 
	{
		fd = -1;
		if (redir->target == 1)
		{
			perror("error");
			return ;
		}
		if (redir->type == RED_INPUT)
			fd = open(redir->data, O_RDONLY);
		else if (redir->type == RED_TRUNK)
		{
			fd = open(redir->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (redir->type == RED_APPEND)
			fd = open(redir->data,  O_CREAT | O_APPEND, 0644);
		else if (redir->type == HER_DOC)
		{
			fd = redir->out_fd;
			if (fd == -1)
			{
				printf("hervalue################### [%d] \n", fd);
			}
		}
		if (fd == -1)
		{
			perror("redirection open failed");
			exit(EXIT_FAILURE);
		}
		if (redir->type == RED_INPUT || redir->type == HER_DOC)
		{
			dup2(fd, STDIN_FILENO);
		}
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}

