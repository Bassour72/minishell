/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:16:51 by massrayb          #+#    #+#             */
/*   Updated: 2024/11/11 05:31:18 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_buffer_size(long number)
{
	int	i;

	i = 0;
	if (number == 0)
		return (1);
	if (number < 0)
	{
		i++;
		number *= -1;
	}
	while (number > 0)
	{
		i++;
		number *= 0.1;
	}
	return (i);
}

static char	*converting(long nbr, char *dst, int size)
{
	int	i;

	i = 0;
	if (nbr < 0)
	{
		dst[0] = '-';
		i++;
		nbr *= -1;
	}
	while (i <= size)
	{
		dst[size] = (nbr % 10) + '0';
		nbr *= 0.1;
		size--;
	}
	return (dst);
}

char	*ft_itoa(int n)
{
	char	*resault;
	int		buff_size;

	buff_size = get_buffer_size((long)n);
	resault = (char *)malloc((sizeof(char) * buff_size) + 1);
	if (resault == 0)
		return (0);
	if (n != 0)
		resault = converting((long)n, resault, buff_size - 1);
	else
		resault[0] = '0';
	resault[buff_size] = '\0';
	return (resault);
}
