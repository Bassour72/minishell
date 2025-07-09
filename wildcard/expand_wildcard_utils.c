/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:47:46 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 16:16:50 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

int	is_wildcard(char *str)
{
	int	literal_string;

	literal_string = -1;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			literal_string = -literal_string;
		if (*str == '*' && literal_string == -1)
			return (1);
		str++;
	}
	return (0);
}

int	init_needle_len(char *name, int start)
{
	int	len;
	int	i;

	len = 0;
	i = start - 1;
	while (name[++i] && name[i] != '*')
		len++;
	return (len);
}

int	is_match_quote(char str_i, char file_name_j)
{
	return ((str_i == DOUBLE_QUOTE && file_name_j == '\"') || \
	(str_i == SINGLE_QUOTE && file_name_j == '\''));
}
