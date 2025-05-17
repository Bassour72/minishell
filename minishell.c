
#include "include/execution.h"

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

int main(int ac, char **av, char **env)
{
	atexit(f);
	t_tree	*tree = NULL;
	char	*input;
	input = NULL;
	while (1)
	{
		//get the input from the terminal
		input = readline("minishell$ ");
		// printf("address : %p\n", input);
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
		t_env *env_l = NULL;
		env_generate(&env_l, env);
		expand_all(env_l, tree);
		print_tree(tree, 0);


		//lable =^=^=^=^=^=^=^=^=^=^=

		//  execution(tree, env);
		// if (tree->data)
		// 	printf("here echo command built-in [%s]\n",tree->data[0]);
		 free_tree(tree); //note this is for freeing the tree
		// take the tree and execute here

		
		//-----------------------------
	}

	return (0);
}