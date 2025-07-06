 #include "../../include/execution.h"
 
static int exec_block_command(t_tree *root,  t_env **env_list, int in_subshell)
{
	if (root->data && expand(&root->data, *env_list) == R_FAIL)
		return (1);
	if (in_subshell)
	{
		run_command(root, env_list);
		check_non_interactive_exit(root, env_list, 1, true);
	}
	if (root->data && is_builtin(root->data[0]) == 0)
	{
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
			return (1);
		return (exec_builtin_command(root, env_list));
	}
	return (exec_external_command(root, env_list));
}

static int handle_and_operator(t_tree *root, t_env **env_list)
{
	int status;

	status = exec_tree(root->left, env_list, 0,  false);
	if (status == 0)
		return (exec_tree(root->right, env_list, 0, false));
	return (status);
}

static int handle_or_operator(t_tree *root, t_env **env_list)
{
	int status;

	status = exec_tree(root->left, env_list,0, false);
	if (status != 0)
		return (exec_tree(root->right, env_list, 0, false));
	return (status);
}

int exec_tree(t_tree *root, t_env **env_list, int in_subshell,  bool is_child)
{
	if (!root)
		return (1);
	if (root->type == BLOCK && root->data == NULL && root->left)
		return (exec_subshell(root, env_list));
	if (root->type == BLOCK)
		return (exec_block_command(root, env_list, in_subshell));
	if (root->type == PIPE)
		return (exec_pipe(root,env_list, is_child));
	if (root->type == OP_AND)
		return (handle_and_operator(root, env_list));
	if (root->type == OP_OR)
		return (handle_or_operator(root, env_list));
	return (1);
}
