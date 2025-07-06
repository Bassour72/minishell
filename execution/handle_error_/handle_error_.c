#include "../../include/execution.h"



void display_error(char *sms_error, const char *target)
{
	char *temp_sms_error;
	char *temp_sms_error_;

	temp_sms_error = NULL;
	temp_sms_error_ = NULL;

	temp_sms_error = ft_strjoin(target, sms_error);
	if (!temp_sms_error)
		return (perror("malloc:"));

	temp_sms_error_ = ft_strjoin("minishell: ", temp_sms_error);
	free(temp_sms_error);

	if (!temp_sms_error_)
		return (perror("malloc:"));
	write(2, temp_sms_error_, ft_strlen(temp_sms_error_));
	free(temp_sms_error_);
}

int handle_dot_commands(char *cmd, bool should_print, bool has_path)
{
	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		if (should_print)
			display_error(".: filename argument required.: usage: . filename [arguments]\n", cmd);
		return (EX_USAGE);
	}
	if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '\0')
	{
		if (has_path)
		{
			if (should_print)
				display_error(": command not found\n", cmd);
			return (STATUS_NOT_FOUND);
		}
		else
		{
			if (should_print)
				display_error("is a directory\n", cmd);
			return (STATUS_IS_DIR);
		}
	}
	return (EXIT_SUCCESS);
}

int handle_error_with_slash(char *cmd, bool should_print)
{
		struct stat st;

		if (stat(cmd, &st) == -1)
		{
		if (should_print)
					perror("minishell");
				return (STATUS_NOT_FOUND);
		}
		if (S_ISDIR(st.st_mode))
		{
			if (should_print)
			 		display_error(": Is a directory\n", cmd);
				return (STATUS_IS_DIR );
		}
		if (access(cmd, X_OK) == -1)
		{
			if (should_print)
					perror("minishell");
				return (CMD_CANNOT_EXECUTE);
		}
		return (EXIT_SUCCESS);
}

int handle_error_no_slash(char *cmd, char *resolved_path, bool should_print)
{
		if (resolved_path != NULL)
		{
			if (should_print)
					display_error(": command not found\n", cmd);
				return (STATUS_NOT_FOUND);
		}
		if (access(cmd, X_OK) == -1)
		{	
			if (should_print)
					perror("minishell");
				return (CMD_CANNOT_EXECUTE);
		}
		return (EXIT_SUCCESS);
}

int should_display_error(char *cmd, t_env **env_list, bool should_print)
{
		bool has_slash;
		char *path;
		int dot_status;

		has_slash = false;

		path = get_env_value("PATH", *env_list);
		if (ft_strchr(cmd, '/'))
				has_slash = true;
		dot_status = handle_dot_commands(cmd, should_print, path);
		if (dot_status)
			return dot_status;
		if (!has_slash)
		{
			return handle_error_no_slash(cmd, path,  should_print);
		}
		else
		{
			return  handle_error_with_slash(cmd,  should_print);
		}
}
