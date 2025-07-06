#include "../../include/execution.h"

void execute_external_command(t_tree *root,t_env **env_list) 
{
	char **new_env;
	char *binary_path;
	int exit_status;

	if (!root->data || !root->data[0])
	{
		// close(0);
		// close(1);
		free_env_list(*env_list);
		free_tree(root);
		exit(0);
	}
	binary_path = get_binary_file_path(root,env_list);
	if (!binary_path) 
	{	
		exit_status =  should_display_error(root->data[0], env_list, true);
		check_non_interactive_exit(root, env_list, exit_status);
	}
	new_env = gen_new_env(*env_list);
	free_env_list(*env_list);
	close_heredoc_fds(root,root->redirections);
	execve(binary_path, root->data, new_env);
	perror("execve");
	check_non_interactive_exit(root, env_list, 1);
}


void run_command(t_tree *root,  t_env **env_list) 
{
	int status;

	if (!root) 
	{
		//close(1); // update here
		// ft_putendl_fd("Error: Empty command node \n", STDERR_FILENO);
		free_tree(root);
		free_env_list(*env_list);
		exit(EXIT_FAILURE);
	}
	if (expand_redir(root->redirections, *env_list) == R_FAIL)
				return ;
	if (apply_redirections(root->redirections,env_list) == 1)
	{
		free_tree(root);
		return ;
	}
		// should if the function failure
	if (is_builtin(root->data[0]) == 0) 
	{
		status = execute_builtin(root,  env_list);
		check_non_interactive_exit(root, env_list, status);
	}
	execute_external_command(root, env_list);
	check_non_interactive_exit(root, env_list, 1);
}

void close_all_fds_except_std(void)
{
	for (int fd = 3; fd < 1024; ++fd)
		close(fd);
}

void check_non_interactive_exit(t_tree *root,  t_env **env_list, int exit_status)
{
    if (!isatty(STDIN_FILENO))
    {
        close(STDIN_FILENO);
    }

    if (!isatty(STDOUT_FILENO))
    {
        close(STDOUT_FILENO);
    }
	close_heredoc_fds(root,root->redirections);
	free_env_list(*env_list);
	free_tree(root);
	close_all_fds_except_std();
	exit(exit_status);
}


// static void	handle_command_not_found(t_tree *root, t_env **env_list);
// static char	*get_or_exit_if_invalid_binary(t_tree *root, t_env **env_list);
// static void	final_exec_or_fail(char *path, t_tree *root, char **envp);

// void	execute_external_command(t_tree *root, t_env **env_list)
// {
// 	char	*binary_path;
// 	char	**new_env;

// 	if (!root->data || !root->data[0])
// 		handle_command_not_found(root, env_list);
// 	binary_path = get_or_exit_if_invalid_binary(root, env_list);
// 	new_env = gen_new_env(*env_list);
// 	free_env_list(*env_list);
// 	close_heredoc_fds(root, root->redirections);
// 	final_exec_or_fail(binary_path, root, new_env);
// }
// static void	handle_command_not_found(t_tree *root, t_env **env_list)
// {
// 	close(0);
// 	close(1);
// 	free_env_list(*env_list);
// 	free_tree(root);
// 	exit(127);
// }
// static char	*get_or_exit_if_invalid_binary(t_tree *root, t_env **env_list)
// {
// 	char	*binary_path;
// 	int		exit_status;

// 	binary_path = get_binary_file_path(root, env_list);
// 	if (!binary_path)
// 	{
// 		exit_status = should_display_error(root->data[0], env_list, true);
// 		close_heredoc_fds(root, root->redirections);
// 		close(0);
// 		close(1);
// 		free_env_list(*env_list);
// 		free_tree(root);
// 		exit(exit_status);
// 	}
// 	return (binary_path);
// }
// static void	final_exec_or_fail(char *path, t_tree *root, char **envp)
// {
// 	execve(path, root->data, envp);
// 	perror("execve");
// 	free(path);
// 	free_tree(root);
// 	exit(EXIT_FAILURE);
// }
