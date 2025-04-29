/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:07:53 by massrayb          #+#    #+#             */
/*   Updated: 2025/04/23 13:22:21 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char **append_command(char **old_list, char *new_cmd)
{
	int	size;
	int	i;
	int	j;

	char **new_list;
	if (!new_cmd) //todo maybe i dont need this
		return (printf("Error: check append command |temp|\n"), NULL);
	//LABLE : build new cmd list from 0
	if (!old_list)
	{
		new_list = malloc(sizeof(char *) * 2);
		if (!new_list)
			return (NULL);
		*new_list = ft_strdup(new_cmd);
		*(new_list + 1) = NULL;
		return (new_list);
	}
	//lable : fill new list with old list + new cmd
	size = 0;
	while (old_list[size++])
		;
		
	new_list = malloc(sizeof(char *) * (size + 1));
	if (!new_list)
		return (NULL);
	i = -1;
	while (old_list[++i])
		new_list[i] = ft_strdup(old_list[i]);
	new_list[i] = ft_strdup(new_cmd);
	new_list[i + 1] = NULL;
	return new_list;
}
