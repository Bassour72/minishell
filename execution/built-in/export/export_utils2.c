/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:47:00 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/06 22:58:07 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

void	update_existing_env(t_env *existing, char *new_value, int append_mode)
{
	char	*temp_value;

	if (new_value)
	{
		if (append_mode)
		{
			temp_value = ft_strjoin(existing->value, new_value);
			free(existing->value);
			free(new_value);
			existing->value = temp_value;
			return ;
		}
		free(existing->value);
		existing->value = new_value;
	}
}

char	*get_env_key(const char *identifier)
{
	char	*new_key;
	int		len;
	int		i;

	len = 0;
	if (!identifier)
		return (NULL);
	while (identifier[len] && identifier[len] != '=' && identifier[len] != '+')
		len++;
	new_key = malloc(len + 1);
	if (!new_key)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_key[i] = identifier[i];
		i++;
	}
	new_key[i] = '\0';
	return (new_key);
}

char	*get_env_value1(const char *identifier)
{
	int	i;

	i = 0;
	if (!identifier)
		return (NULL);
	while (identifier[i] && identifier[i] != '=')
		i++;
	if (!identifier[i])
		return (NULL);
	return (ft_strdup(identifier + i + 1));
}

void	swap_node(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;
	bool	temp_exported;

	if (!a || !b)
		return ;
	temp_key = a->key;
	temp_value = a->value;
	temp_exported = a->exported;
	a->key = b->key;
	a->value = b->value;
	a->exported = b->exported;
	b->key = temp_key;
	b->value = temp_value;
	b->exported = temp_exported;
}
