
#include "include/execution.h"
int g_exit_status = 0; 
/* Special handling for SHLVL*/
int update_env_shlvl(t_env **env_list, long value);
int env_add_back(t_env **env_list, const char *key, const char *value);
int is_valid_shlvl_string(char *str)
{
	int i = 0;

	if (!str || !*str)
		return 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return 0;
		i++;
	}
	return 1;
}
int env_add_back(t_env **env_list, const char *key, const char *value)
{
    t_env *new;
    t_env *last;

    new = malloc(sizeof(t_env));
    if (!new)
        return (1);
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = NULL;
    if (!new->key || !new->value)
        return (free(new->key), free(new->value), free(new), 1);
    if (!*env_list)
        *env_list = new;
    else
    {
        last = *env_list;
        while (last->next)
            last = last->next;
        last->next = new;
    }
    return (0);
}

long parse_shlvl(char *str)
{
	if (!is_valid_shlvl_string(str))
		return 1;
	return ft_atoi(str);
}

long compute_next_shlvl(long current)
{
	if (current < 0)
		return 0;
	if (current >= 1000)
	{
		//todo 
		fprintf(stderr, "bash: warning: shell level (%ld) too high, resetting to 1\n", current);
		return 1;
	}
	return current + 1;
}

int update_env_shlvl(t_env **env_list, long value)
{
	char *new_value;
	t_env *tmp;

	if (!env_list)
		return 1;
	new_value = ft_itoa(value);
	if (!new_value)
		return 1;
	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			free(tmp->value);
			tmp->value = new_value;
			// printf("\033[32msss->%s\033[0m\n", tmp->value);
			return 0;
		}
		tmp = tmp->next;
	}
	// Add new SHLVL if not found
	//return env_add_back(env_list, "SHLVL", new_value);
	return 0;
}

int should_increment_shlvl(char *program_path)
{
	//printf("\n\n\n\n   ======[%s]==== \n\n\n", program_path);
	if (ft_strcmp(program_path, "./minishell") == 0)
	{
		// printf("\n\n\n\n   ======###[%s]==== \n\n\n", program_path);
		return (0);
	}
	else
		return (1);
}

int handle_shlvl(char *argv0, t_env **env_list)
{
	char *shlvl_str;
	long current, next;
	
	if (should_increment_shlvl(argv0))
		return 0;
	if (!env_list || !*env_list)
		return 1;
	shlvl_str = env_get_value(*env_list, "SHLVL");
	//printf("\n\n\n\n  shlvl_str ======[%s]==== \n\n\n", shlvl_str);
	current = parse_shlvl(shlvl_str);
	// printf("\n\n\n\n  current ======[%ld]==== \n\n\n", current);
	next = compute_next_shlvl(current);
	//printf("\n\n\n\n  next ======[%ld]==== \n\n\n", next);
	return update_env_shlvl(env_list, next);
}

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
	while (*str && *str == ' ')
	{
		str++;
	}
	return (!*str);
}

void f()
{
	system("valgrind --leak-check=full --show-leak-kinds=all ./minishell");
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
	atexit(f);
	t_tree	*tree = NULL;
	t_env *env_list = NULL;	
	char	*input;
	input = NULL;

	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);

	
	// printf("if has null [%p]\n", *env);
	// printf("herer\n");
	env_generate(&env_list, env);
	handle_shlvl(av[0],&env_list);
	while (1)
	{
		//get the input from the terminal
		input = readline("minishell$ ");
		//printf("address : %p\n", input);
		if (!input)
			break ;
		if (empty(input))
		{
			free(input);
			continue ;
		}
	
		add_history(input);

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




		//lable =^=^=^=^=^=^=^=^=^=^=
		//print_env(env_list);
		execution(tree, env, &env_list);
		// if (tree->data)
		// 	printf("here echo command built-in [%s]\n",tree->data[0]);
		//  free_tree(tree); //note this is for freeing the tree
		// take the tree and execute here

		
		//-----------------------------
	}
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