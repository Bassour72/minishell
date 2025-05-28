
#include "include/execution.h"
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
	if (ft_strcmp(program_path, "./minishell") == 0)
	{
		printf("\n\n\n\n   ======[%s]==== \n\n\n", program_path);
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
	printf("\n\n\n\n  shlvl_str ======[%s]==== \n\n\n", shlvl_str);
	current = parse_shlvl(shlvl_str);
	printf("\n\n\n\n  current ======[%lld]==== \n\n\n", current);
	next = compute_next_shlvl(current);
	printf("\n\n\n\n  next ======[%lld]==== \n\n\n", next);
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
	system("leaks -q minishell");
}

void expand_all(t_env *env, t_tree *root)
{
	if (root->type == BLOCK)
		expand(env, root);
	if (root->left)
		expand_all(env, root->left);
	if (root->right)
		expand_all(env, root->right);
}

//todo for debug
// void handle_exit(int sig) 
// {
//     print_memory_leaks();
//     exit(0);
// }
int	init_env(t_env **env_list)
{
	t_env *tmp;
	tmp = malloc(sizeof(t_env));
	tmp->key = ft_strdup("OLDPWD");
	tmp->value = NULL;
	tmp->next = NULL;
	/******************************** */
	tmp->next = malloc(sizeof(t_env));
	tmp->next->key = ft_strdup("PWD");
	tmp->next->value = ft_strdup("/home/ybassour/Desktop/minishell");;
	tmp->next->next = NULL;
	/********************************************* */
	tmp->next->next = malloc(sizeof(t_env));
	tmp->next->next->key = ft_strdup("SHLVL");
	tmp->next->next->value = ft_strdup("1");;
	tmp->next->next->next = NULL;
	*env_list = tmp;
}
int main(int ac, char **av, char **env)
{
	//atexit(f);
	t_tree	*tree = NULL;
	t_env *env_list = NULL;	
	char	*input;
	input = NULL;
	// signal(SIGINT, handle_exit);
    // signal(SIGTERM, handle_exit);
	printf("if has null [%p]\n", *env);
	printf("herer\n");
	if (env_generate(&env_list, env) == -1)
		init_env(&env_list);
	print_env(env_list);
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

		tree = parser(tree, input);
		if (!tree)
			continue;
		// lable parsing tests========
		//  t_env *env_l = NULL;
		//  env_generate(&env_l, env);
		// expand_all(env_l, tree);
		print_tree(tree, 0);


		//lable =^=^=^=^=^=^=^=^=^=^=
		
		execution(tree, env, &env_list);
		// if (tree->data)
		// 	printf("here echo command built-in [%s]\n",tree->data[0]);
		 //free_tree(tree); //note this is for freeing the tree
		// take the tree and execute here

		
		//-----------------------------
	}

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