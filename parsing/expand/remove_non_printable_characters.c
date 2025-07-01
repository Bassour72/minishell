/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_non_printable_characters.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:36:33 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 22:40:13 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"

static int	allocate_double_arr_size(char ***new_data, char **old_data)
{
	int	size;

	size = -1;
	while (*(old_data + ++size))
		;
	*new_data = malloc(sizeof(char *) * (size + 1));
	if (!new_data)
		return (perror("error: "), R_FAIL);
	while (size >= 0)
		*(*new_data + size--) = NULL;
	return (R_SUCCESS);
}

static int	generate_new_data_str(char **dst, char *str)
{
	int	size;
	int	i;

	i = -1;
	size = 0;
	while (*(str + ++i))
	{
		if (*(str + i) != DOUBLE_QUOTE && *(str + i) != SINGLE_QUOTE)
			size++;
	}
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	i = 0;
	while (*str)
	{
		if (*str != DOUBLE_QUOTE && *str != SINGLE_QUOTE)
			*(*dst + i++) = *str;
		str++;
	}
	*(*dst + i) = '\0';
	return (R_SUCCESS);
}

int	remove_non_printable_characters(char ***old_data)
{
	char	**new_data;
	int		i;

	if (allocate_double_arr_size(&new_data, *old_data) == R_FAIL)
		return (free_2d_arr(*old_data), *old_data = NULL, R_FAIL);
	i = -1;
	while (*(*old_data + ++i))
	{
		if (generate_new_data_str(new_data + i, *(*old_data + i)) == R_FAIL)
		{
			(free_2d_arr(new_data), free_2d_arr(*old_data));
			return (R_FAIL);
		}
	}
	free_2d_arr(*old_data);
	*old_data = new_data;
	return (R_SUCCESS);
}
