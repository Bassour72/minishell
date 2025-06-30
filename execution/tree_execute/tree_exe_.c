 #include "../../include/execution.h"
// int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell)
// {
// 	if (!root)
// 		return (1);
// 	if (root->type == BLOCK && root->data == NULL && root->left)
// 		return exec_subshell_block(root, env, env_list);
// 	if (root->type == BLOCK)
// 		return exec_command_block(root, env, env_list, input_fd, in_subshell);
// 	if (root->type == PIPE)
// 		return exec_pipe(root, env, input_fd, env_list);
// 	if (root->type == OP_AND || root->type == OP_OR)
// 		return exec_logical_op(root, env, env_list);
// 	return (1);
// }
// int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell)
// {
// 	if (!root)
// 		return (1);
// 	if (root->type == BLOCK && root->data == NULL && root->left)
// 		return (exec_subshell(root, env, env_list));
// 	if (root->type == BLOCK)
// 		return (exec_block_command(root, env, env_list, input_fd, in_subshell));
// 	if (root->type == PIPE)
// 		return (exec_pipe(root, env, input_fd, env_list));
// 	if (root->type == OP_AND)
// 		return (handle_and_operator(root, env, env_list));
// 	if (root->type == OP_OR)
// 		return (handle_or_operator(root, env, env_list));
// 	return (1);
// }

int wait_child_status(int input_fd, pid_t pid, t_env **env_list)
{
	int status;
	int sig;

	if (input_fd != STDIN_FILENO)
		close(input_fd);

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) //true if child exited normally.
	{
		int s = WTERMSIG(status); 
		sig = WEXITSTATUS(status); // — gives the exit code. // Gets exit code (if WIFEXITED is true)
		return (sig);
	}
	if (WIFSIGNALED(status)) // true if child was terminated by a signal.
	{
		 sig = WTERMSIG(status); // Which signal caused termination
		return (sig + 128);
	}
	return (1);
}
//todo not subshell it parenthese
static int exec_subshell(t_tree *root, char **env, t_env **env_list)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
			exit(1);
		if (apply_redirections(root->redirections, env_list) == 1)
			exit(1);
		exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static int exec_builtin_command(t_tree *root, char **env, t_env **env_list)
{
	int saved_stdin;
	int saved_stdout;
	int status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(root->redirections, env_list) == 1)
		return (1);
	status = execute_builtin(root, env, env_list);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

static int exec_external_command(t_tree *root, char **env, t_env **env_list, int input_fd)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
			exit(1);
		if (apply_redirections(root->redirections, env_list) == 1)
			exit(1);
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		execute_external_command(root, env_list);
		exit(EXIT_FAILURE);
	}
	return (wait_child_status(input_fd, pid, env_list));
}

static int exec_block_command(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell)
{
	if (root->data && expand(&root->data, *env_list) == R_FAIL)
		return (1);
	if (in_subshell)
	{
		if (input_fd != STDIN_FILENO)
		{
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
		}
		run_command(root, env, env_list);
		exit(EXIT_FAILURE);
	}
	if (root->data && is_builtin(root->data[0]) == 0)
	{
		if (expand_redir(root->redirections, *env_list) == R_FAIL)
			return (1);
		return exec_builtin_command(root, env, env_list);
	}
	return exec_external_command(root, env, env_list, input_fd);
}

static int handle_and_operator(t_tree *root, char **env, t_env **env_list)
{
	int status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
	if (status == 0)
		return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
	return status;
}

static int handle_or_operator(t_tree *root, char **env, t_env **env_list)
{
	int status = exec_tree(root->left, env, env_list, STDIN_FILENO, 0);
	if (status != 0)
		return exec_tree(root->right, env, env_list, STDIN_FILENO, 0);
	return status;
}

int exec_tree(t_tree *root, char **env, t_env **env_list, int input_fd, int in_subshell)
{
	if (!root)
		return (1);
	if (root->type == BLOCK && root->data == NULL && root->left)
		return (exec_subshell(root, env, env_list));
	if (root->type == BLOCK)
		return (exec_block_command(root, env, env_list, input_fd, in_subshell));
	if (root->type == PIPE)
		return (exec_pipe(root, env, input_fd, env_list));
	if (root->type == OP_AND)
		return (handle_and_operator(root, env, env_list));
	if (root->type == OP_OR)
		return (handle_or_operator(root, env, env_list));
	return (1);
}
