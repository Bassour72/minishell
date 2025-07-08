/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:55:34 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 17:48:00 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	is_builtin(char	*command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (0);
	if (ft_strcmp(command, "cd") == 0)
		return (0);
	if (ft_strcmp(command, "env") == 0)
		return (0);
	if (ft_strcmp(command, "exit") == 0)
		return (0);
	if (ft_strcmp(command, "export") == 0)
		return (0);
	if (ft_strcmp(command, "pwd") == 0)
		return (0);
	if (ft_strcmp(command, "unset") == 0)
		return (0);
	return (1);
}

int	execute_builtin(t_tree *root, t_env **env_list)
{
	if (ft_strcmp(root->data[0], "echo") == 0)
		return (builtin_echo(root));
	if (ft_strcmp(root->data[0], "cd") == 0)
		return (cd_change_working_directory(root, env_list));
	if (ft_strcmp(root->data[0], "pwd") == 0)
		return (pwd_print_working_directory(root, env_list));
	if (ft_strcmp(root->data[0], "env") == 0)
		return (env_environment(root, *env_list));
	if (ft_strcmp(root->data[0], "exit") == 0)
		return (exit_exe(root, env_list));
	if (ft_strcmp(root->data[0], "export") == 0)
		return (export_command_builtin(root, env_list));
	if (ft_strcmp(root->data[0], "unset") == 0)
		return (builtin_unset_environment(root, env_list));
	return (1);
}
