/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:58:54 by massrayb          #+#    #+#             */
/*   Updated: 2024/11/09 10:04:49 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	src_i;
	size_t	dst_len;

	src_i = 0;
	if (size == 0)
		return (ft_strlen(src));
	dst_len = ft_strlen(dst);
	if (dst_len >= size)
		return (size + ft_strlen(src));
	if (src[src_i] && src_i < size - dst_len - 1)
		ft_strlcpy(dst + dst_len, src, size - dst_len);
	return (dst_len + ft_strlen(src));
}
