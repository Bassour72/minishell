#include "../../include/execution.h"


// int check_valid_(char *command)
// {
// 	int i;
// 	int is_space;

// 	if (!command)
// 		return (0);
// 	i = 0;
// 	is_space = 0;
// 	while (command[i] != '\0')
// 	{
// 		if (command[i] != ' ' && command[i] != '\t')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }


void display_error_has_slash( char *cmd ,char *path)
{
		if (access(cmd, F_OK) != 0 && !path)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd,         STDERR_FILENO);
            ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        }
		else if (access(cmd, X_OK) != 0 && !path)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd,         STDERR_FILENO);
            ft_putendl_fd(": Permission **********************************************denied", STDERR_FILENO);
        }
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd,         STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
		}
}

void display_error_no_slash( char *cmd ,char *path)
{
  if (access(cmd, F_OK) != 0)
    {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd,         STDERR_FILENO);
            ft_putendl_fd(": No such file or directory", STDERR_FILENO);
    }
     else if (access(cmd, X_OK) != 0)
    {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd,         STDERR_FILENO);
            ft_putendl_fd(": Permission denied", STDERR_FILENO);
			//should return beacuse the exit of permission denied it 126
    }
}
void should_display_error(t_tree *root, t_env **env_list)
{
    char *cmd;
    bool has_slash;
    char *path;

	has_slash = false;
	cmd = root->data[0];
	path = get_env_value("PATH", *env_list);
	if (ft_strchr(cmd, '/'))
		has_slash = true;
    if (!has_slash)
    {
		 display_error_has_slash( cmd ,path);
		// if (access(cmd, F_OK) != 0 && !path)
        // {
        //     ft_putstr_fd("minishell: ", STDERR_FILENO);
        //     ft_putstr_fd(cmd,         STDERR_FILENO);
        //     ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        // }
		// else if (access(cmd, X_OK) != 0 && !path)
        // {
        //     ft_putstr_fd("minishell: ", STDERR_FILENO);
        //     ft_putstr_fd(cmd,         STDERR_FILENO);
        //     ft_putendl_fd(": Permission denied", STDERR_FILENO);
        // }
		// else
		// {
		// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
		// 	ft_putstr_fd(cmd,         STDERR_FILENO);
		// 	ft_putendl_fd(": command not found", STDERR_FILENO);
		// }
    }
    else
    {
		display_error_no_slash( cmd ,path);
        // if (access(cmd, F_OK) != 0)
        // {
        //     ft_putstr_fd("minishell: ", STDERR_FILENO);
        //     ft_putstr_fd(cmd,         STDERR_FILENO);
        //     ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        // }
        // else if (access(cmd, X_OK) != 0)
        // {
        //     ft_putstr_fd("minishell: ", STDERR_FILENO);
        //     ft_putstr_fd(cmd,         STDERR_FILENO);
        //     ft_putendl_fd(": Permission denied", STDERR_FILENO);
        // }
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
		return ;
	}
	binary_path = get_binary_file_path(root,env_list);
	if (!binary_path) 
	{	
		should_display_error(root, env_list);
		close(0);
		close(1);
		free_env_list(*env_list);
		free_tree(root);
		exit(127);
	}
	new_env = gen_new_env(*env_list);
	free_env_list(*env_list);
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
