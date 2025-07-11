/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:27:38 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/11 15:48:08 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	int		sign;
	long	result;

	if (!str)
		return (-1);
	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
		result = result * 10 + (*str++ - '0');
	return (result * sign);
}

static int  check_has_only(t_tree *root)
{
	int i;

	i = 1;
	while (root->data[i] != NULL)
	{
		if (!is_numeric(root->data[i]))
			return (STATUS_OK);
		i++;
	}
	return (STATUS_ERROR);
}

int	exit_exe(t_tree *root, t_env **env_list)
{
	long	code;

	if (!root || !root->data)
		check_non_interactive_exit(root, env_list, EXIT_FAILURE, true);
	if (!root->is_forked)
		write(1, "exit\n", 5);
	if (root->data[1] == NULL)
	{
		code = ft_atol(get_env_value("exit_status@gmail.com", *env_list));
		check_non_interactive_exit(root, env_list, code, true);
		exit(code);
	}
	if (!is_numeric(root->data[1]) && root->data[2] != NULL && !check_has_only(root))
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		check_non_interactive_exit(root, env_list, 255, true);
	}
	if (root->data[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (STATUS_ERROR);
	}
	code = ft_atol(root->data[1]);
	check_non_interactive_exit(root, env_list, (unsigned char)code, true);
	return (STATUS_ERROR);
}
