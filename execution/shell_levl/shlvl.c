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

long compute_next_shlvl(long current)
{
	if (current < 0)
		return (0);
	if (current >= 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
		ft_putnbr_fd(current, STDERR_FILENO);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
		return (1);
	}
	return (current + 1);
}

int update_env_shlvl(t_env **env_list, long value)
{
	char *new_value;
	t_env *tmp;

	if (!env_list)
		return (1);
	new_value = ft_itoa(value);
	if (!new_value)
		return (1);
	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			free(tmp->value);
			tmp->value = new_value;
			return (0);
		}
		tmp = tmp->next;
	}
	//todo Add new SHLVL if not found
	//return env_add_back(env_list, "SHLVL", new_value);
	return (0);
}

int should_increment_shlvl(char *program_path)
{
	if (ft_strcmp(program_path, "./minishell") == 0)
	{
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
		return (0);
	if (!env_list || !*env_list)
		return (1);
	shlvl_str = env_get_value(*env_list, "SHLVL");
	current = parse_shlvl(shlvl_str);
	next = compute_next_shlvl(current);
	return (update_env_shlvl(env_list, next));
}
