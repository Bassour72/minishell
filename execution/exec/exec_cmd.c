#include "../../include/execution.h"

void display_error(char *sms_error, char *target)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putendl_fd(sms_error, STDERR_FILENO);
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


void execute_external_command(t_tree *root,t_env **env_list) 
{
	char **new_env;
	char *binary_path;
	int exit_status;

	if (!root->data || !root->data[0])
	{
		close(0);
		close(1);
		free_env_list(*env_list);
		free_tree(root);
		exit(127);
	}
	binary_path = get_binary_file_path(root,env_list);
	if (!binary_path) 
	{	
		exit_status =  should_display_error(root->data[0], env_list, true);
		close_heredoc_fds(root,root->redirections);
		close(0);
		close(1);
		free_env_list(*env_list);
		free_tree(root);
		exit(exit_status);
	}
	new_env = gen_new_env(*env_list);
	free_env_list(*env_list);
	close_heredoc_fds(root,root->redirections);
	execve(binary_path, root->data, new_env);
	perror("execve");
	free(binary_path);
	free_tree(root);
	exit(EXIT_FAILURE);
}


void run_command(t_tree *root,  t_env **env_list) 
{
	int status;
	if (!root || !root->data || !root->data[0]) 
	{
		// ft_putendl_fd("Error: Empty command node \n", STDERR_FILENO);
		//close(1);
		free_tree(root);
		exit(EXIT_FAILURE);
	}
	if (expand_redir(root->redirections, *env_list) == R_FAIL)
				return ;
	if (apply_redirections(root->redirections,env_list) == 1)
	{
		free_tree(root);
		//free_env(env_list);
		return;
	}
		// should if the function failure
	if (is_builtin(root->data[0]) == 0) 
	{
		status = execute_builtin(root,  env_list);
		free_tree(root);
		exit(status);
	}
	execute_external_command(root, env_list);
	exit(EXIT_FAILURE);
}


