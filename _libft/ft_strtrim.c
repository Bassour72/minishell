/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:04:40 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/09 16:19:10 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_a_set(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	starting(char *s, char *set, size_t start)
{
	while (s[start] && is_a_set(s[start], set))
		start++;
	return (start);
}

static int	ending(char *s, char *set, size_t end)
{
	while (s[end] && is_a_set(s[end], set))
		end--;
	return (end + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*resault;
	size_t	start;
	size_t	end;
	size_t	s;

	start = 0;
	if (s1 == 0 || set == 0)
		return (NULL);
	start = starting((char *)s1, (char *)set, start);
	end = ft_strlen((char *)s1) - 1;
	if (ft_strlen(s1) == start)
	{
		resault = (char *)ft_calloc(1, 1);
		return (resault);
	}
	else
	{
		end = ending((char *)s1, (char *)set, end);
		s = (ft_strlen((char *)s1) - start) - (ft_strlen((char *)s1) - end);
		resault = (char *)malloc((s * sizeof(char)) + 1);
		if (resault == 0)
			return (NULL);
		ft_strlcpy(resault, (char *)s1 + start, s + 1);
	}
	return (resault);
}
