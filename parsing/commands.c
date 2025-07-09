/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:09:14 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 15:17:05 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

static char	**create_new_list(char *new_cmd)
{
	char	**new_list;

	new_list = malloc(sizeof(char *) * 2);
	if (!new_list)
		return (perror("error"), NULL);

	new_list[0] = ft_strdup(new_cmd);
	if (new_list[0] == NULL)
		return (perror("error"), free(new_list), NULL);
	*(new_list + 1) = NULL;
	return (new_list);
}

static char	**copy_old_list_to_new(char **old_list, char **new_list, int *_i)
{
	int	i;

	i = -1;
	while (old_list[++i])
	{
		new_list[i] = ft_strdup(old_list[i]);
		if (!new_list[i])
		{
			free_2d_arr(old_list);
			free_2d_arr(new_list);
			return (perror("error"), NULL);
		}
	}
	return (*_i = i, new_list);
}

char	**append_command(char **old_list, char *new_cmd)
{
	int		size;
	int		i;
	char	**new_list;

	if (!old_list)
		return (create_new_list(new_cmd));
	size = 0;
	while (old_list[size++])
		;
	new_list = malloc(sizeof(char *) * (size + 1));
	if (!new_list)
		return (perror("error"), free_2d_arr(old_list), NULL);
	if (copy_old_list_to_new(old_list, new_list, &i) == NULL)
		return (NULL);
	free_2d_arr(old_list);
	new_list[i] = ft_strdup(new_cmd);
	if (new_list[i] == NULL)
		return (perror("error"), free_2d_arr(new_list), NULL);
	new_list[i + 1] = NULL;
	return (new_list);
}
