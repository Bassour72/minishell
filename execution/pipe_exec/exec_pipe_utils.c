/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:32:12 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 18:02:13 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

void	close_pipe_fds(int pipefd[2])
{
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
}

int	create_pipe(int pipefd[2], t_tree *root, t_env **env_list, bool is_child)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		if (is_child)
		{
			check_non_interactive_exit(root, env_list, -1, true);
		}
		return (-1);
	}
	return (0);
}
