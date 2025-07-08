/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:48:33 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 17:44:44 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

static void	add_new_env_node(t_env **env_list, char *new_key, char *new_value)
{
	t_env	*new_node;
	t_env	*ptr;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = new_key;
	new_node->value = new_value;
	new_node->exported = true;
	new_node->next = NULL;
	if (!*env_list)
		*env_list = new_node;
	else
	{
		ptr = *env_list;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new_node;
	}
}

int	add_env_without_appned(t_env **env_list, char *new_key, char *new_value)
{
	t_env	*existing;

	existing = is_exist_env(*env_list, new_key);
	if (existing)
	{
		update_existing_env(existing, new_value, 0);
		free(new_key);
		return (1);
	}
	add_new_env_node(env_list, new_key, new_value);
	return (1);
}

int	append_env_node(t_env **env_list, char *new_key, char	*new_value)
{
	t_env	*existing;

	existing = is_exist_env(*env_list, new_key);
	if (existing)
	{
		update_existing_env(existing, new_value, 1);
		free(new_key);
		return (1);
	}
	add_new_env_node(env_list, new_key, new_value);
	return (1);
}

static int	add_env(char *arg, t_env **env_list)
{
	char	*new_key;
	char	*new_value;
	int		is_append_mode;

	if (!arg)
		return (1);
	is_append_mode = 0;
	if (is_valid_identifier(arg, &is_append_mode) == 1)
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	new_key = get_env_key(arg);
	if (!new_key)
		return (1);
	new_value = get_env_value1(arg);
	if (is_append_mode != 1)
	{
		return (add_env_without_appned(env_list, new_key, new_value));
	}
	return (append_env_node(env_list, new_key, new_value));
}

int	export_command_builtin(t_tree *root, t_env **env_list)
{
	int	i;

	if (env_list == NULL)
		return (0);
	else if (root->data[1] != NULL)
	{
		i = 1;
		while (root->data[i] != NULL)
		{
			add_env(root->data[i], env_list);
			update_env_exit_status(env_list, 1);
			i++;
		}
		return (0);
	}
	else
		print_env_export_sort(*env_list, root);
	return (0);
}
