#include "../../include/execution.h"

int is_builtin(char *command) 
{
	if (strcmp(command, "echo") == 0)
		return (0);
	if (strcmp(command, "cd") == 0)
		return (0);
	if (strcmp(command, "env") == 0)
		return (0);
	if (strcmp(command, "exit") == 0)
		return (0);
	if (strcmp(command, "export") == 0)
		return (0);
	if (strcmp(command, "pwd") == 0)
		return (0);
	if (strcmp(command, "unset") == 0)
		return (0);
	return (1);
}


int execute_builtin(t_tree *root,  t_env **env_list)
{
	if (strcmp(root->data[0], "echo") == 0)
		return (builtin_echo(root));
	if (strcmp(root->data[0], "cd") == 0)
		return (cd_change_working_directory(root, env_list));
	if (strcmp(root->data[0], "pwd") == 0)
		return (pwd_print_working_directory(root, env_list));
	if (strcmp(root->data[0], "env") == 0)
		return (env_environment(root, *env_list));
	if (strcmp(root->data[0], "exit") == 0)
	{
		exit_exe(root, env_list);
		return (0);
	}
	if (strcmp(root->data[0], "export") == 0)
		return (export_command_builtin(root, env_list));
	if (strcmp(root->data[0], "unset") == 0)
		return (builtin_unset_environment(root, env_list));
	return (1);
}
