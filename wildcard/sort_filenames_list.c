/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_filenames_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:19:14 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 14:49:13 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static void	swap_filename_node(t_wc_node *a, t_wc_node *b)
{
	char	*data;
	int		private;

	if (!a || !b)
		return ;
	data = a->data;
	private = a->private;
	a->data = b->data;
	a->private = b->private;
	b->data = data;
	b->private = private;
}

static int	is_swapable(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_wc_node	*sort_filenames(t_wc_node *env_list)
{
	bool		swapped;
	t_wc_node	*ptr;

	if (!env_list)
		return (NULL);
	swapped = true;
	while (swapped)
	{
		swapped = false;
		ptr = env_list;
		while (ptr->next)
		{
			if (is_swapable(ptr->data, ptr->next->data) > 0)
			{
				swap_filename_node(ptr, ptr->next);
				swapped = true;
			}
			ptr = ptr->next;
		}
	}
	return (env_list);
}
