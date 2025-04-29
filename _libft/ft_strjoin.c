/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:39:41 by massrayb          #+#    #+#             */
/*   Updated: 2024/11/17 10:05:10 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*resault;
	size_t	s1_len;
	size_t	s2_len;

	if (s1 == 0 && s2 == 0)
		return (NULL);
	if (s1 == 0)
		return (ft_strdup(s2));
	if (s2 == 0)
		return (ft_strdup(s1));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	resault = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (resault == 0)
		return (NULL);
	ft_strlcpy(resault, s1, s1_len + 1);
	ft_strlcpy(resault + s1_len, s2, s2_len + 1);
	return (resault);
}
