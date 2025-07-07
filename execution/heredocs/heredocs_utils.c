/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:21:07 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/07 22:02:30 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

void	close_heredoc_fds(t_tree *root, t_red *redir)
{
	t_red	*r;

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

void	propagate_fork_flag(t_tree *root, int is_forked)
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

int	count_heredocs(t_tree *node)
{
	int		total;
	t_red	*r;

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

void	enforce_heredoc_limit(t_tree *root, t_env **env_list)
{
	int	heredocs;

	heredocs = count_heredocs(root);
	if (heredocs > MAX_HEREDOC)
	{
		write(STDERR_FILENO, \
		"bash: maximum here-document count exceeded\n", 44);
		free_env_list(*env_list);
		free_tree(root);
		exit(2);
	}
}

int	prepare_heredocs(t_tree *root, t_env **env_list)
{
	t_red	*redir;

	if (!root)
		return (0);
	redir = root->redirections;
	if (expand_herdoc_delimiter(redir, *env_list) == R_FAIL)
		return (1);
	while (redir)
	{
		if (redir->type == HER_DOC)
		{
			if (create_heredoc(redir, env_list))
				return (1);
			if (g_exit_status == 1)
				return (1);
		}
		if (g_exit_status == 1)
			return (1);
		redir = redir->next;
	}
	if (prepare_heredocs(root->left, env_list))
		return (1);
	if (prepare_heredocs(root->right, env_list))
		return (1);
	return (0);
}
