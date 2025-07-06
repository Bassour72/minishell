#include "../../include/execution.h"

int is_valid_shlvl_string(char *str)
{
	int i = 0;

	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
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
		return (1);
	return (ft_atoi(str));
}