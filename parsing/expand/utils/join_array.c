/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:50:45 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 13:38:51 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	calculate_len(char **arr)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (arr[i])
	{
		len += ft_strlen(arr[i]);
		if (arr[i + 1])
			len += 1;
		i++;
	}
	return (len);
}

static void	cpy_to_single_arr(char **double_arr, char **single_arr, int len)
{
	int	i;

	i = -1;
	while (double_arr[++i])
	{
		ft_strlcat(*single_arr, double_arr[i], len);
		if (double_arr[i + 1])
			ft_strlcat(*single_arr, " ", len);
	}
}

int	join_arr(char **arr, char **joined_arr)
{
	int	len;

	len = calculate_len(arr);
	*joined_arr = malloc(len + 1);
	if (!*joined_arr)
		return (perror("error"), R_FAIL);
	ft_bzero(*joined_arr, len + 1);
	cpy_to_single_arr(arr, joined_arr, len + 1);
	return (R_SUCCESS);
}
