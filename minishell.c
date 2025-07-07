#include "include/execution.h"
int g_exit_status = 0; 

void f()
{
	system("lsof -c minishell");
	// system("ls -l /proc/self/fd");
	//system("ls -l /proc/$$/fd");
	//system("ls -l /proc/self/fd > fd_log.txt"); // Or print per command with identifier

	//system("valgrind --leak-check=full --show-leak-kinds=all ./minishell");
}

void	handle_sigint_prompt(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == 0)
		return;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 1;
}

void	enable_echoctl(void)
{
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return;
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	disable_echoctl(void)
{
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	empty(char *str)
{
	if (!str[0])
		return (1);
	while (*str && ft_isspace(*str))
		str++;
	return (!*str);
}

int handle_heredoc(t_tree *root, t_env **env_list)
{
	enforce_heredoc_limit(root, env_list);
	return (prepare_heredocs(root, env_list));
}
static void	is_input_null(char *input, t_env *env_list)
{
	int status;

	if (input == NULL)
	{
		write(1, "exit\n", 5);
		close(2);
		close(0);
		status = ft_atoi(get_env_value("exit_status@gmail.com", env_list));
		free_env_list(env_list);
		//free_tree(tree);
		exit(status);
	}
}

static int	shell_loop(t_env **env_list)
{
	t_tree	*tree;
	char	*input;
	int		status;

	while (1)
	{
		signal(SIGINT, handle_sigint_prompt);
		disable_echoctl();
		if (dup2(2, 0) == -1)
			return (R_FAIL);
		input = readline("minishell$ ");
		enable_echoctl();

		if (g_exit_status == 1)
		{
			update_env_exit_status(env_list, 1);
			g_exit_status = 0;
		}
		is_input_null(input, *env_list);
		add_history(input);
		if (empty(input))
			continue;
		add_history(input);
		if (parser(&tree, input, env_list) == R_FAIL)
			return (1);
		if (!tree)
			continue;
		if (handle_heredoc(tree, env_list) != 0)
		{
			update_env_exit_status(env_list, 1);
			close_heredoc_fds(tree, tree->redirections);
			free_tree(tree);
			g_exit_status = 0;
			continue;
		}
		status = execution(tree, env_list);
		update_env_exit_status(env_list, status);
		free_tree(tree);
		g_exit_status = 0;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_list = NULL;
	int		status;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		exit (1);

	if (env_generate(&env_list, envp) == R_FAIL)
		return (1);
	if (handle_shlvl(av[0], &env_list) == R_FAIL)
		return (1);
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	status = shell_loop(&env_list);
	free_env_list(env_list);
	return (status);
}

