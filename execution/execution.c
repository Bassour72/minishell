/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:52:05 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 21:07:55 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

int	is_forkred(t_tree *root)
{
	if (root == NULL)
		return (0);
	if (root->type == PIPE)
		return (1);
	if (is_forkred(root->left))
		return (1);
	if (is_forkred(root->right))
		return (1);
	return (0);
}

int	execution(t_tree *root, t_env **env_list)
{
	int	status;

	if (!root)
		return (1);
	if (is_forkred(root))
		propagate_fork_flag(root, 1);
	else
		propagate_fork_flag(root, 0);
	status = exec_tree(root, env_list, 0, false);
	close_heredoc_fds(root);
	g_exit_status = 0;
	return (status);
}
