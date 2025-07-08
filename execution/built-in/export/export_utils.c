/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:38:59 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 18:42:46 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

int	ft_strcmp_v2(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static int	is_valid_first_char(const char *identifier)
{
	if (!identifier)
		return (STATUS_OK);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (STATUS_OK);
	return (STATUS_ERROR);
}

static int	validate_identifier_chars(const char *identifier, \
			int *is_append_mode)
{
	int	i;
	int	count_plus;

	i = 1;
	count_plus = 0;
	while (identifier[i] != '\0' && identifier[i] != '=')
	{
		if (identifier[i] == '+')
		{
			if (identifier[i + 1] == '=')
				count_plus++;
			else
				return (STATUS_OK);
		}
		else if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (STATUS_OK);
		if (count_plus > 1)
			return (STATUS_OK);
		i++;
	}
	*is_append_mode = count_plus;
	return (STATUS_ERROR);
}

int	is_valid_identifier(const char *identifier, int *is_append_mode)
{
	if (!is_valid_first_char(identifier))
		return (STATUS_ERROR);
	if (!validate_identifier_chars(identifier, is_append_mode))
		return (STATUS_ERROR);
	return (STATUS_OK);
}
