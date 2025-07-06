#include "../../include/execution.h"

void display_error(char *sms_error, char *target)
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

int handle_error_with_slash(char *cmd, bool should_print)
{
    struct stat st;

    if (stat(cmd, &st) == -1)
    {
		if (should_print)
        	perror("minishell");
        return 127;
    }
    if (S_ISDIR(st.st_mode))
    {
		if (should_print)
			 display_error(": Is a directory\n", cmd);
        return 126;
    }
    if (access(cmd, X_OK) == -1)
    {
		if (should_print)
        	perror("minishell");
        return 126;
    }
    return 0;
}

int handle_error_no_slash(char *cmd, char *resolved_path, bool should_print)
{
    if (resolved_path != NULL)
    {
		if (should_print)
			display_error(": command not found\n", cmd);
        return 127;
    }
    if (access(cmd, X_OK) == -1)
    {	if (should_print)
        	perror("minishell");
        return 126;
    }
    return 0;
}

int should_display_error(char *cmd, t_env **env_list, bool should_print)
{
    bool has_slash;
    char *path;

	has_slash = false;

	path = get_env_value("PATH", *env_list);
	if (ft_strchr(cmd, '/'))
		has_slash = true;
    if (!has_slash)
    {
		return handle_error_no_slash(cmd, path,  should_print);
    }
    else
    {
		return  handle_error_with_slash(cmd,  should_print);
    }
}
