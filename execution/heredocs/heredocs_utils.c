
#include "../../include/execution.h"

void close_heredoc_fds(t_tree *root, t_red *redir)
{
	t_red *r;

	r = redir;
	if (root == NULL)
		return ;
	while (r)
	{
		if (r->type == HER_DOC && r->out_fd >= 0)
		{
			close(r->out_fd);
			r->out_fd = -1;
		}
		r = r->next;
	}
	close_heredoc_fds(root->left, redir);
	close_heredoc_fds(root->right, redir);

}

void propagate_fork_flag(t_tree *root, int is_forked)
{
	if (!root)
		return ;
	root->is_forked = is_forked;
	if (root->type == PIPE)
	{
		propagate_fork_flag(root->left, 1);
		propagate_fork_flag(root->right, 1);
	}
	else
	{
		propagate_fork_flag(root->left, is_forked);
		propagate_fork_flag(root->right, is_forked);
	}
}

int count_heredocs(t_tree *node)
{
	int total;
	t_red *r;

	total = 0;
	if (!node)
		return (0);
	r = node->redirections;
	while (r)
	{
		if (r->type == HER_DOC)
			total++;
		r = r->next;
	}
	total += count_heredocs(node->left);
	total += count_heredocs(node->right);
	return (total);
}


void enforce_heredoc_limit(t_tree *root, t_env **env_list)
{
	int heredocs;

	heredocs = count_heredocs(root);
	if (heredocs > MAX_HEREDOC)
	{
		write(STDERR_FILENO, "bash: maximum here-document count exceeded\n",44);
		free_env_list(*env_list);
		free_tree(root);
		exit(2);
	}
}