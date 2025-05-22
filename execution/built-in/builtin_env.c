#include "../../include/execution.h"

int	env_environment(t_tree *root, char **env)
{
	t_env *env_list = NULL;
	env_generate(&env_list, env);
	//env_generate(env_list, env);
	print_env(env_list);
	free_env_list(env_list);
}