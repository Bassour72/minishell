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
	//system("leaks ./minishell");
	system("ls -l /proc/self/fd");
	//system("ls -l /proc/$$/fd");
	//system("ls -l /proc/self/fd > fd_log.txt"); // Or print per command with identifier

	//system("valgrind --leak-check=full --show-leak-kinds=all ./minishell");
}


void update_env_exit_status(t_env **env_list, int status)
{
    char *str = ft_itoa(status);
    update_last_executed_command(env_list, "exit_status@gmail.com", str);
}

void print_debugg(char **env)
{
	while (*env != NULL)
	{
		printf("===[%s]\n", *env);
		// sleep(2);

		env++;
	}
	
}

//todo for debug
void handle_sigint_prompt(int sig)
{
    (void)sig;
   rl_replace_line("", 0);
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
   rl_redisplay();
    g_exit_status = 130;
}


int main(int ac, char **av, char **env)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (1);
		//printf("stdin is a TTY\n");
	// else
	// {
	// 	//printf("stdin is a PIPE or redirected\n");
	// 	exit(0);
	// }


	t_tree	*tree = NULL;
	t_env *env_list = NULL;	
	char	*input;
	input = NULL;
	int status;
	char *exit_str;
	int stdin_backup;
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);

	env_generate(&env_list, env);
	handle_shlvl(av[0],&env_list);
	while (1)
	{
		signal(SIGINT, handle_sigint_prompt);
		input = readline("minishell$ ");
		//stdin_backup = dup(0);
		// print_debugg(env);
		if (g_exit_status == 130)
		{
			update_env_exit_status(&env_list, 130);
			g_exit_status = 0;
		}
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			close(2);
			close(0);
			status =ft_atoi( get_env_value("exit_status@gmail.com", env_list));
			free_env_list(env_list);
			//free_tree(tree);
			exit(status);
		}

		add_history(input);

		if (empty(input))
		{
			free(input);
			continue ;
		}
	
		

		if (parser(&tree, input) == R_FAIL)
		{
			return (1);
		}
		if (!tree)
			continue;
		// lable parsing tests========
		//  t_env *env_l = NULL;
		//  env_generate(&env_l, env);
		// print_tree(tree, 0);
		// expand_redir(tree->redirections, env_list);
		// printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		status = execution(tree,&env_list);
		g_exit_status = status;
		exit_str = ft_itoa(status);
		update_last_executed_command(&env_list, "exit_status@gmail.com", exit_str);
		free_tree(tree);
		// for (int fd = 3; fd <= 50; ++fd)
    	// {
		// 	if (!isatty(fd));
		// 		close(fd);
		// }
		// close(STDIN_FILENO);
		// dup2(stdin_backup, STDIN_FILENO);
		// close(stdin_backup);
		//-----------------------------
	}
	// stdin_backup = dup(STDERR_FILENO);
	// close(stdin_backup);
	// 	// do some redirection...
	// dup2(stdin_backup, STDIN_FILENO);
	free_env_list(env_list);

	return (0);
}

/*
int main(int argc, char **argv, char **envp)
{
    t_env *env_list;

    // Step 1: Convert envp[] to linked list
    env_list = convert_envp_to_list(envp);

    // ✅ Step 2: Handle SHLVL before doing anything else
    handle_shlvl(argv[0], &env_list);

    // Step 3: Continue with initializing readline, signals, etc.
    shell_loop(&env_list);

    return (0);
}

*/