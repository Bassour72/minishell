/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_exec_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:48:56 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 13:24:35 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	wait_child_status( pid_t pid)
{
	int	status;
	int	sig;

	if (pid > 0)
		waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		sig = WEXITSTATUS(status);
		return (sig);
	}
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		return (sig + 128);
	}
	return (1);
}

int	exec_subshell(t_tree *root, t_env **env_list)
{
	pid_t	pid;
	int		status;
	int		exit_tree;

	pid = fork();
	if (pid < 0)
		return (perror("fork :"), -1);
	if (pid == 0)
	{
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
		{
			check_non_interactive_exit(root, env_list, 1, true);
		}
		if (apply_redirections(root->redirections, env_list) == 1)
		{
			check_non_interactive_exit(root, env_list, 1, true);
		}
		exit_tree = exec_tree(root->left, env_list, 0, true);
		check_non_interactive_exit(root, env_list, exit_tree, true);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	exec_builtin_command(t_tree *root, t_env **env_list)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(root->redirections, env_list) == 1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	status = execute_builtin(root, env_list);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

int	exec_external_command(t_tree *root, t_env **env_list)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork :"), -1);
	if (pid == 0)
	{
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
		{
			check_non_interactive_exit(root, env_list, 1, true);
		}
		if (apply_redirections(root->redirections, env_list) == 1)
		{
			check_non_interactive_exit(root, env_list, 1, true);
		}
		execute_external_command(root, env_list);
		check_non_interactive_exit(root, env_list, 1, true);
	}
	return (wait_child_status(pid));
}
