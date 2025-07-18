/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error_.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:15:08 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 20:52:24 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

int	display_error(char *sms_error, const char *target, int return_error)
{
	char	*temp_sms_error;
	char	*temp_sms_error_;

	temp_sms_error = NULL;
	temp_sms_error_ = NULL;
	temp_sms_error = ft_strjoin(target, sms_error);
	if (!temp_sms_error)
		return (perror("malloc:"), EXIT_MALLOC_FAIL);
	temp_sms_error_ = ft_strjoin("minishell: ", temp_sms_error);
	free(temp_sms_error);
	if (!temp_sms_error_)
		return (perror("malloc:"), EXIT_MALLOC_FAIL);
	write(2, temp_sms_error_, ft_strlen(temp_sms_error_));
	free(temp_sms_error_);
	return (return_error);
}

int	handle_dot_commands(char *cmd, bool should_print, bool has_path)
{
	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		if (should_print)
			return (display_error(DOT_USAGE_ERROR, cmd, EX_USAGE));
		return (EX_USAGE);
	}
	if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '\0')
	{
		if (has_path)
		{
			if (should_print)
				return (display_error(": command not found\n", cmd, \
							CMD_NOT_FOUND));
			return (CMD_NOT_FOUND);
		}
		else
		{
			if (should_print)
				return (display_error("is a directory\n", cmd, STATUS_IS_DIR));
			return (STATUS_IS_DIR);
		}
	}
	return (STATUS_OK);
}

int	handle_error_with_slash(char *cmd, bool should_print)
{
	struct stat	st;

	if (stat(cmd, &st) == -1)
	{
		if (should_print)
			perror("minishell");
		return (STATUS_NOT_FOUND);
	}
	if (S_ISDIR(st.st_mode))
	{
		if (should_print)
			display_error(": Is a directory\n", cmd, STATUS_IS_DIR);
		return (STATUS_IS_DIR);
	}
	if (access(cmd, X_OK) == -1)
	{
		if (should_print)
			perror("minishell");
		return (CMD_CANNOT_EXECUTE);
	}
	return (STATUS_OK);
}

int	handle_error_no_slash(char *cmd, char *resolved_path, bool should_print)
{
	if (resolved_path != NULL)
	{
		if (should_print)
			display_error(": command not found\n", cmd, STATUS_NOT_FOUND);
		return (STATUS_NOT_FOUND);
	}
	if (access(cmd, X_OK) == -1)
	{
		if (should_print)
			return (display_error(": No such file or directory\n", \
						cmd, STATUS_NOT_FOUND));
		return (STATUS_NOT_FOUND);
	}
	return (STATUS_OK);
}

int	should_display_error(char *cmd, t_env **env_list, bool should_print)
{
	bool	has_slash;
	char	*path;
	int		dot_status;

	has_slash = false;
	path = get_env_value("PATH", *env_list);
	if (ft_strchr(cmd, '/'))
		has_slash = true;
	dot_status = handle_dot_commands(cmd, should_print, path);
	if (dot_status)
		return (dot_status);
	if (!has_slash)
	{
		return (handle_error_no_slash(cmd, path, should_print));
	}
	else
	{
		return (handle_error_with_slash(cmd, should_print));
	}
}
