/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:37:13 by massrayb          #+#    #+#             */
/*   Updated: 2025/03/18 04:46:57 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_white_spaces(const char *s, int i)
{
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' || s[i] == '\f'
		|| s[i] == '\r' || s[i] == '\n')
		i++;
	return (i);
}

static int	handle_sign(const char *s, int index, int *sign)
{
	if (s[index] == '+' || s[index] == '-')
	{
		if (s[index] == '-')
			*sign *= -1;
		index++;
	}
	return (index);
}

static long	handle_numbers(const char *s, int index, int sign)
{
	long	res;

	res = 0;
	while (s[index] >= '0' && s[index] <= '9')
	{
		res *= 10;
		res += (s[index] - '0');
		index++;
		if ((res * sign) > INT_MAX || (res * sign) < INT_MIN)
			return (LONG_MAX);
	}
	return (res * sign);
}

long	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	i = handle_white_spaces(str, i);
	i = handle_sign(str, i, &sign);
	result = handle_numbers(str, i, sign);
	return (result);
}
