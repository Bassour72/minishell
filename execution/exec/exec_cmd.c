#include "../../include/execution.h"

void execute_external_command(t_tree *root,t_env **env_list) 
{
	char **new_env;
	char *binary_path;
	//printf("inside the execute external command \n ");
	binary_path = get_binary_file_path(root,env_list);
	if (!binary_path) 
	{	
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(root->data[0], STDERR_FILENO);
		ft_putendl_fd(": Command not found: ",STDERR_FILENO);
		close(0);
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

void run_command(t_tree *root, char **env, t_env **env_list) 
{
	int status;
	if (!root || !root->data || !root->data[0]) 
	{
		fprintf(stderr, "Error: Empty command node \n");
		free_tree(root);
		exit(EXIT_FAILURE);
	}
	// if (expand_redir(root->redirections, *env_list) == R_FAIL)
	// 			return ;
	if (apply_redirections(root->redirections,env_list) == 1)
		return;
		// should if the function failure
	if (is_builtin(root->data[0]) == 0) 
	{
		status = execute_builtin(root, env, env_list);
		free_tree(root);
		exit(status);
	}
	execute_external_command(root, env_list);
	exit(EXIT_FAILURE); //todo Unreachable
}
