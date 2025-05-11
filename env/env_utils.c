#include "../include/parsing.h"

char *env_get_value(t_env *list, char *key)
{
	if (!key || !list)
		return (NULL);
	while (list)
	{
		if (ft_strlen(key) == ft_strlen(list->key) && ft_strnstr(list->key, key, ft_strlen(key)))
			return (list->value);
		list = list->next;
	}
	return (NULL);
}

void free_env_list(t_env *list)
{
	t_env *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}