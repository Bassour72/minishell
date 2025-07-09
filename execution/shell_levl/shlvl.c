/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:38:57 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/09 14:39:51 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

static long	compute_next_shlvl(long current)
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

static int	update_env_shlvl(t_env **env_list, long value)
{
	char	*new_value;
	t_env	*tmp;

	if (!env_list)
		return (1);
	new_value = ft_itoa(value);
	if (!new_value)
		return (perror("malloc: "), R_FAIL);
	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			free(tmp->value);
			tmp->value = new_value;
			return (R_SUCCESS);
		}
		tmp = tmp->next;
	}
	free(new_value);
	return (R_SUCCESS);
}

int	should_increment_shlvl(char *program_path)
{
	if (ft_strcmp(program_path, "./minishell") == 0 || \
	ft_strcmp(program_path, "minishell") == 0)
	{
		return (0);
	}
	else
		return (1);
}

static long	parse_shlvl(char *str)
{
	int		i;
	long	current;

	i = 0;
	if (!str || !*str)
		return (1);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	current = ft_atol(NULL);
	return (current);
}

int	handle_shlvl(char *argv0, t_env **env_list)
{
	char	*shlvl_str;
	long	current;
	long	next;

	if (should_increment_shlvl(argv0))
		return (R_SUCCESS);
	if (!env_list || !*env_list)
		return (R_FAIL);
	shlvl_str = env_get_value(*env_list, "SHLVL");
	current = parse_shlvl(shlvl_str);
	next = compute_next_shlvl(current);
	if (update_env_shlvl(env_list, next) == R_FAIL)
	{
		free_env_list(*env_list);
		return (R_FAIL);
	}
	return (R_SUCCESS);
}
