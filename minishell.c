#include "include/execution.h"
int g_exit_status = 0; 

// t_env *add_envp_to_node()
// t_env *convert_envp_to_list(char **envp)
// {
// 	t_env *envp_list;
// 	if (!envp)
// 		return (NULL);
	
// }
int	empty(char *str)
{
	if (!str[0])
		return 1;
	while (*str && ft_isspace(*str))
	{
		str++;
	}
	return (!*str);
}

void f()
{
	system("lsof -c minishell");
	// system("ls -l /proc/self/fd");
	//system("ls -l /proc/$$/fd");
	//system("ls -l /proc/self/fd > fd_log.txt"); // Or print per command with identifier

	//system("valgrind --leak-check=full --show-leak-kinds=all ./minishell");
}

//todo for debug
void handle_sigint_prompt(int sig)
{
    (void)sig;
	if (waitpid(-1, NULL, WNOHANG) == 0)
			return ;
    write(STDOUT_FILENO, "\n", 1);
   rl_replace_line("", 0);
    rl_on_new_line();
   rl_redisplay();
    g_exit_status = 130;
}
void enable_echoctl(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
        return;
    term.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disable_echoctl(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
        return;
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
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

static void	protect_is_a_tty()
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		exit (1);
}

static void	first_initialization(char **input, t_env **env_list, char **env, char *app_name)
{
	*input = NULL;
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	if (env_generate(env_list, env) == R_FAIL)
		exit(1);
	handle_shlvl(app_name,env_list);
}
int main(int ac, char **av, char **env)
{
	protect_is_a_tty();
	t_tree	*tree = NULL;
	t_env	*env_list = NULL;	
	char	*input;
	int		status;

	first_initialization(&input, &env_list, env, av[0]);
	while (1)
	{
		signal(SIGINT, handle_sigint_prompt);
		disable_echoctl();
		input = readline("minishell$ ");
		enable_echoctl();
		if (g_exit_status == 130)
		{
			update_env_exit_status(&env_list, 130);
			g_exit_status = 0;
		}
		is_input_null(input, env_list);
		add_history(input);
		if (empty(input))
		{
			free(input);
			continue ;
		}
		if (parser(&tree, input, &env_list) == R_FAIL)
			return (1);
		if (!tree)
			continue;
		status = execution(tree,&env_list);
		printf("\n\n\n\n [%d] \n\n\n", status);
		update_env_exit_status(&env_list, status);
		free_tree(tree);
		g_exit_status = 0;
	}
	free_env_list(env_list);
	return (0);
}
