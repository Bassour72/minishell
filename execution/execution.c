
#include "../include/execution.h"

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
		exit(-1); // call for 
	close_heredoc_fds(root,root->redirections);
	g_exit_status = 0;
	return (status);
}
