/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:50:50 by massrayb          #+#    #+#             */
/*   Updated: 2024/11/21 13:50:43 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	s_size;
	char			*resault;

	i = 0;
	if (!s || !f)
		return (NULL);
	s_size = ft_strlen(s);
	resault = malloc((s_size * sizeof(char)) + 1);
	if (resault == 0)
		return (NULL);
	while (i < s_size)
	{
		resault[i] = f(i, s[i]);
		i++;
	}
	resault[i] = 0;
	return (resault);
}
