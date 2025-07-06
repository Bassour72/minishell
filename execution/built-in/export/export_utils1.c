/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:44:42 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/06 22:46:31 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

t_env	*is_exist_env(t_env *env_list, const char *new_key)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, new_key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	list_env_add_back(t_env **env_list, t_env *new_node_env)
{
	t_env	*copy;

	if (!new_node_env)
		return ;
	if (!(*env_list))
		*env_list = new_node_env;
	else
	{
		copy = *env_list;
		while (copy->next != NULL)
			copy = copy->next;
		copy->next = new_node_env;
	}
}

t_env	*copy_env_list(t_env *env_list)
{
	t_env	*copy;
	t_env	*new_node;

	copy = NULL;
	while (env_list != NULL)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = NULL;
		new_node->value = NULL;
		if (env_list->key)
			new_node->key = ft_strdup(env_list->key);
		if (env_list->value)
			new_node->value = ft_strdup(env_list->value);
		new_node->exported = env_list->exported;
		new_node->next = NULL;
		list_env_add_back(&copy, new_node);
		env_list = env_list->next;
	}
	return (copy);
}

t_env	*sort_env_list(t_env *env_list)
{
	bool	swapped;
	t_env	*ptr;

	if (!env_list)
		return (NULL);
	swapped = true;
	while (swapped)
	{
		swapped = false;
		ptr = env_list;
		while (ptr->next)
		{
			if (ft_strcmp_v2(ptr->key, ptr->next->key) > 0)
			{
				swap_node(ptr, ptr->next);
				swapped = true;
			}
			ptr = ptr->next;
		}
	}
	return (env_list);
}

void	print_env_export_sort(t_env *env_list)
{
	t_env	*sorted_list;
	t_env	*tmp;

	sorted_list = copy_env_list(env_list);
	sorted_list = sort_env_list(sorted_list);
	tmp = sorted_list;
	while (tmp)
	{
		if (tmp->value)
		{
			if (tmp->exported)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		}
		else
		{
			if (tmp->exported)
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}
	free_env_list(sorted_list);
}
