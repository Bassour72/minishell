#include "../../include/execution.h"

static int	is_numeric(const char *str)
{
	int i = 0;

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

static long	ft_atol(const char *str)
{
	int		sign = 1;
	long	result = 0;

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

int	exit_exe(t_tree *root, t_env **env_list)
{
	long	code;

	
	if (!root || !root->data)
        exit(0);
    if (!root->is_forked)
	    write(1, "exit\n", 5);
    if (root->data[1] == NULL)
	{
		code =ft_atoi( get_env_value("exit_status@gmail.com", *env_list));
		free_env_list(*env_list);
		free_tree(root);
		// close(1);
		// close(0);

        exit(code);
	}
	if (!is_numeric(root->data[1]))
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		free_env_list(*env_list);
		free_tree(root);
		// 		close(1);
		// close(0);
		exit(255);
	}
	if (root->data[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	code = ft_atol(root->data[1]);
	free_env_list(*env_list);
	free_tree(root);
		// 	close(1);
		// close(0);
	exit((unsigned char)code);
}
