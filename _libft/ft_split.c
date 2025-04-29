/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:09:00 by massrayb          #+#    #+#             */
/*   Updated: 2025/03/12 20:25:03 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_strings(char *str, char c)
{
	size_t	i;

	i = 0;
	while (*str != '\0')
	{
		if (*str != c)
		{
			while (*str != c && *str)
				str++;
			i++;
		}
		else
			str++;
	}
	return (i);
}

static char	*custom_strdub(char *s, size_t len)
{
	char	*res;

	res = (char *)malloc((len + 1) * sizeof(char));
	if (res == 0)
		return (NULL);
	ft_strlcpy(res, s, len + 1);
	return (res);
}

static char	**free_store(int i, char **store)
{
	while (i >= 0)
	{
		free(store[i]);
		store[i] = NULL;
		i--;
	}
	free(store);
	return (NULL);
}

static char	**do_split(char **store, char *s, char c, int size)
{
	size_t	start;
	int		i;
	int		j;

	start = 0;
	i = 0;
	j = 0;
	while (j < size)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		store[j] = custom_strdub(s + start, i - start);
		if (store[j] == NULL)
			return (free_store(j - 1, store));
		j++;
	}
	return (store);
}

char	**ft_split(char const *s, char c)
{
	char	**store;
	size_t	size;

	if (s == NULL)
		return (NULL);
	size = count_strings((char *)s, c);
	store = (char **)malloc((size + 1) * sizeof(char *));
	if (store == NULL)
		return (NULL);
	store = do_split(store, (char *)s, c, size);
	if (store != NULL)
		store[size] = NULL;
	return (store);
}
