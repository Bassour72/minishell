#include "../include/execution.h"



//todo remove this 
//fixme 

void write_heredoc(int fd, const char *limiter) {
	char *line;
	while (1) {
		line = readline("> ");
		if (!line || strcmp(line, limiter) == 0) {
			free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void create_heredoc(t_red *redir) {
	char tmp_path[] = "/tmp/heredocXXXXXX";
	int fd = mkstemp(tmp_path);
	if (fd == -1) {
		perror("heredoc creation failed");
		exit(EXIT_FAILURE);
	}
	write_heredoc(fd, redir->target);
	close(fd);
	redir->heredoc_path = strdup(tmp_path);
}

void prepare_heredocs(t_tree *root) 
{
	if (!root) return;
	t_red *redir = root->redirections;
	while (redir) 
	{
		if (redir->type == HER_DOC)
			create_heredoc(redir);
		redir = redir->next;
	}
	prepare_heredocs(root->left);
	prepare_heredocs(root->right);
}

void apply_redirections(t_red *redir) 
{
	while (redir) 
	{
		int fd = -1;
		if (redir->type == RED_INPUT)
			fd = open(redir->target, O_RDONLY);
		// else if (redir->type == RED_OUTPUT)
		// 	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == RED_APPEND)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == HER_DOC)
			fd = open(redir->heredoc_path, O_RDONLY);
		if (fd == -1) {
			perror("redirection open failed");
			exit(EXIT_FAILURE);
		}
		if (redir->type == RED_INPUT || redir->type == HER_DOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}