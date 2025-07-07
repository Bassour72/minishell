/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:09:42 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/06 23:14:25 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

void	execute_external_command(t_tree *root, t_env **env_list)
{
	char	**new_env;
	char	*binary_path;
	int		exit_status;

	if (!root->data || !root->data[0])
	{
		check_non_interactive_exit(root, env_list, 0, true);
	}
	binary_path = get_binary_file_path(root, env_list);
	if (!binary_path)
	{
		exit_status = should_display_error(root->data[0], env_list, true);
		check_non_interactive_exit(root, env_list, exit_status, true);
	}
	new_env = gen_new_env(*env_list);
	free_env_list(*env_list);
	close_heredoc_fds(root, root->redirections);
	execve(binary_path, root->data, new_env);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	perror("execve");
	check_non_interactive_exit(root, env_list, 1, false);
}

void	run_command(t_tree *root, t_env **env_list)
{
	int	status;

	if (!root)
	{
		check_non_interactive_exit(root, env_list, EXIT_FAILURE, true);
	}
	if (expand_redir(root->redirections, *env_list) == R_FAIL)
		return ;
	if (apply_redirections(root->redirections, env_list) == 1)
	{
		free_tree(root);
		return ;
	}
	if (is_builtin(root->data[0]) == 0)
	{
		status = execute_builtin(root, env_list);
		check_non_interactive_exit(root, env_list, status, true);
	}
	execute_external_command(root, env_list);
	check_non_interactive_exit(root, env_list, 1, true);
}

void	close_all_fds_except_std(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		++fd;
	}
}

void	check_non_interactive_exit(t_tree *root, t_env **env_list, \
		int exit_status, bool should_free)
{
	if (!isatty(STDIN_FILENO))
	{
		close(STDIN_FILENO);
	}
	if (!isatty(STDOUT_FILENO))
	{
		close(STDOUT_FILENO);
	}
	close_heredoc_fds(root, root->redirections);
	if (should_free)
		free_env_list(*env_list);
	free_tree(root);
	close_all_fds_except_std();
	exit(exit_status);
}
