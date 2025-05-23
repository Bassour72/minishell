#include "../../include/execution.h"

int remove_env_node(t_env **env_list, const char *target_key)
{
	t_env *tmp;
	t_env *to_remove;

	if (!env_list || !*env_list || !target_key)
		return (1);
	if (ft_strcmp((*env_list)->key, target_key) == 0)
	{
		to_remove = *env_list;
		*env_list = (*env_list)->next;
		free(to_remove->key);
		free(to_remove->value);
		free(to_remove);
		return (0);
	}
	tmp = *env_list;
	while (tmp->next)
	{
		if (ft_strcmp(tmp->next->key, target_key) == 0)
		{
			to_remove = tmp->next;
			tmp->next = tmp->next->next;
			free(to_remove->key);
			free(to_remove->value);
			free(to_remove);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int 	builtin_unset_environment(t_tree *root, t_env **env_list, char **env)
{
	int i;

	if (!root || !env_list)
		return (1);
	i = 1;
	while (root->data[i])
	{
		remove_env_node(env_list, root->data[i]);
		i++;
	}
	return (0);
}
