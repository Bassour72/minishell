/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:07:15 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 14:44:44 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static int	is_there_any_matches(t_wc_node *file_names)
{
	int	len;

	len = 0;
	while (file_names)
	{
		if (file_names->match)
			len++;
		file_names = file_names->next;
	}
	return (len);
}

static int	do_expand_wildcard(t_wc_node **wc_node, t_wc_node *file_names)
{
	t_wc_node	*new_list;

	new_list = NULL;
	init_file_names_matches(file_names, (*wc_node)->data);
	if (is_there_any_matches(file_names) == 0)
		return (R_CONTINUE);
	else
	{
		while (file_names)
		{
			if (file_names->match)
			{
				if (append_wc_node(&new_list, file_names->data, 0) == 0)
					return (free_wc_node_list(new_list), *wc_node = NULL, 0);
			}
			file_names = file_names->next;
		}
		(free((*wc_node)->data), free(*wc_node));
		*wc_node = new_list;
	}
	return (R_SUCCESS);
}

static void	insert_the_expanded_wildcard_to_the_list(t_wc_node **args_list, \
t_wc_node *befor, t_wc_node *after, t_wc_node *wc_node)
{
	if (befor)
		befor->next = wc_node;
	else
		*args_list = wc_node;
	while (wc_node->next)
		wc_node = wc_node->next;
	if (after)
		after->prev = wc_node;
	wc_node->next = after;
}

int	expand_wildcard(t_wc_node **args_list, t_wc_node *file_names)
{
	t_wc_node	*tmp;
	t_wc_node	*wc_node;
	t_wc_node	*befor;
	t_wc_node	*after;
	int			state;

	tmp = *args_list;
	while (tmp)
	{
		wc_node = tmp;
		tmp = tmp->next;
		if (is_wildcard(wc_node->data))
		{
			befor = wc_node->prev;
			after = wc_node->next;
			state = do_expand_wildcard(&wc_node, file_names);
			if (state == R_CONTINUE)
				continue ;
			if (state == R_FAIL)
				return (R_FAIL);
			insert_the_expanded_wildcard_to_the_list(args_list, \
			befor, after, wc_node);
		}
	}
	return (R_SUCCESS);
}
