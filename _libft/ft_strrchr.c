/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:10 by massrayb          #+#    #+#             */
/*   Updated: 2024/11/09 09:59:55 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*res;
	size_t	i;

	i = 0;
	res = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			res = (char *)&s[i];
		i++;
	}
	if ((char)c == s[i])
		res = (char *)&s[i];
	return (res);
}
