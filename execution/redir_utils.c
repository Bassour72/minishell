#include "../include/execution.h"



//todo remove this 
//fixme 

void write_heredoc(int fd, const char *limiter) 
{
	char *line;
	//	sleep(2);
	printf("Write heredoc \n ");
	while (1) 
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0) 
		{
			free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void create_heredoc(t_red *redir) 
{
	sleep(2);
	printf("Create heredoc \n ");
	char tmp_path[] = "/tmp/heredocXXXXXX";
	int fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	//int out_fd = open(tmp_path, O_RDONLY, 0644);
	if (fd == -1) {
		perror("heredoc creation failed");
		exit(EXIT_FAILURE);
	}
	write_heredoc(fd, redir->data);
	close(fd);
	redir->out_fd = 10000;
	//fix remember for unlink the file i create  like this for debugg 
	redir->heredoc_path = ft_strdup(tmp_path); //todo free the memory for the path
}

void prepare_heredocs(t_tree *root) 
{
		//sleep(2);
	printf("Prepare heredoc \n ");
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
	sleep(2);
	printf("apply redirections \n ");
	if (redir == NULL)
		return ;
	printf("check the value of fd is  incrrect value################### [%d] \n", redir->type);
	while (redir) 
	{
		int fd = -1;
		printf("check the value of fd is  incrrect value################### [%d] \n", redir->type);
		if (redir->type == RED_INPUT)
			fd = open(redir->data, O_RDONLY);
		else if (redir->type == RED_TRUNK)
		{
			printf("check the value of fd is  incrrect value################### [%d] \n", redir->type);
			fd = open(redir->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (redir->type == RED_APPEND)
			fd = open(redir->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == HER_DOC)
		{
			//sleep(2);
			printf("here for the if the out_put has the correct value################### [%d] \n", redir->out_fd);
			printf("In the apply redirections fucntion for check the heredoc file and why??\n[%s] program exit!!! \n ", redir->heredoc_path);
			fd = open(redir->heredoc_path, O_RDONLY);
			//sleep(2);
			printf("check the value of fd is  incrrect value################### [%d] \n", fd);
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
			//sleep(2);
			printf("For check why it exit and know the type=========[%d]\n ", redir->type);
			dup2(fd, STDIN_FILENO);
		}
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	//sleep(2);
	printf("end of fucntion for check why it's exit with error \n ");
}