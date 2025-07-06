#include "../include/execution.h"

//todo remove this 
//fixme 
//todo you don't need to share **env with all fucntion 

void setup_exit_handler(int sig)
{
	(void)sig;
    g_exit_status = 130;
	exit(130);
	// close(STDIN_FILENO);
	// write(STDOUT_FILENO, "\n", 1);
}


int is_forkred(t_tree *root)
{
    if (root == NULL)
        return 0;
    if (root->type == PIPE)
        return 1;
    if (is_forkred(root->left))
        return 1;
    if (is_forkred(root->right))
        return 1;
    return 0;
}


// static void	close_all_fds_(void)
// {
// 	int fd;

// 	fd = 3;
// 	while (fd <= 40)
//     {
// 		if (!isatty(fd))
//         {
// 			if (read(fd, "", 0) != -1)
// 			    close(fd);
			
//         }
// 		 ++fd;
// 	}
// }
int execution(t_tree *root,  t_env **env_list)
{
	int status;

	if (!root) 
		return (1);
	if (is_forkred(root))
    	propagate_fork_flag(root, 1);
	else
    	 propagate_fork_flag(root, 0);
    enforce_heredoc_limit(root, env_list);
	status = prepare_heredocs(root, env_list);
	if (status != 0)
	{
        close_heredoc_fds(root,root->redirections);
		if (g_exit_status == 130)
			status = g_exit_status;
		return (status);
	}
	status = exec_tree(root, env_list,0, false);
    if (status == -1)
        exit(-1);
  //  close_all_fds_();
    close_heredoc_fds(root,root->redirections);
    // if (is_forkred(root))
    //     close_all_fds_();
	g_exit_status = 0;
	return (status);
}
