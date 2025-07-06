/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:07:15 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 00:00:59 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static int is_wildcard(char *str)
{
	int literal_string;

	literal_string = -1;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			literal_string = -literal_string;
		if (*str == '*' && literal_string == -1)
			return (1);
		str++;
	}
	return (0);
}

int init_needle_len(char *name, int start)
{
	int len;
	int i;

	len  = 0;
	i = start - 1;
	while (name[++i] && name[i] != '*')
		len++; 

	return (len);
}

int	is_match_quote(char str_i, char file_name_j)
{
	return ((str_i == DOUBLE_QUOTE && file_name_j == '\"') || (str_i == SINGLE_QUOTE && file_name_j == '\''));
}

int is_file_name_match(char *file_name, char *str)
{
	int wc_pos;			// abcd.c
	int	i;			// *a
	int j;
	int file_name_len = ft_strlen(file_name);
	int str_len = ft_strlen(str);
	int is_literal_string = -1;

	wc_pos = -1;
	j = 0;
	i = 0;

	while (1)
	{
		if (str[i] == '\"' || str[i] == '\'')									
			(i++, is_literal_string *= -1);
			
		else if (str[i] == '*' && is_literal_string == -1)
		{
			wc_pos = ++i;
			if (str[i] == '\'' || str[i] == '\"')
				wc_pos = i + 1;								
		}
		
		else if (str[i] != file_name[j])
		{
			if (wc_pos != i)
				i = wc_pos;
			else
				j++;							
			if ((j == file_name_len && wc_pos == i && i != str_len) || wc_pos == -1)
				return (0);
			else if (j == file_name_len && i == str_len)
				return (1);
		}
		else
		{
									
			if (i == str_len && j == file_name_len)
				return (1);
			(i++, j++);
		}
		if (j == file_name_len && i == str_len)
			return (1);
	}
	   
}

void	init_file_names_matches(t_wc_node *file_names, char *name)
{
	while (file_names)
	{
		if (is_file_name_match(file_names->data, name) == 1)
			file_names->match = 1;
		file_names = file_names->next;
	}
}

int do_expand_wildcard(t_wc_node **wc_node, t_wc_node *file_names)
{
	t_wc_node *new_list = NULL;

	init_file_names_matches(file_names, (*wc_node)->data);
	
	int len = 0;
	for(t_wc_node *fn = file_names; fn; fn = fn->next)
		if (fn->match) len++;

	if (len == 0)
	{
		free((*wc_node)->data);
		free(*wc_node);
		*wc_node = NULL;
	}
	else
	{
		for(t_wc_node *fn = file_names; fn; fn = fn->next)
		{
			if (fn->match)
			{
				if (append_wc_node(&new_list, fn->data, 0) == R_FAIL)
					return (free_wc_node_list(new_list), *wc_node = NULL, R_FAIL);
			}
		}
		free((*wc_node)->data);
		free(*wc_node);
		*wc_node = new_list;
	}
	return (R_SUCCESS);
}

int expand_wildcard(t_wc_node **args_list, t_wc_node *file_names)
{
	t_wc_node *tmp;
	t_wc_node *wc_node;
	t_wc_node *befor;
	t_wc_node *after;

	tmp = *args_list;
	while (tmp)
	{

		wc_node = tmp;
		tmp = tmp->next;
		if (is_wildcard(wc_node->data))
		{
			// printf("\033[33m[%s]\033[0m is a wildcard with lenght \033[33m[%d]\033[0m\n", wc_node->data, ft_strlen(wc_node->data));
			befor = wc_node->prev;
			after = wc_node->next;
			if (do_expand_wildcard(&wc_node, file_names) == R_FAIL)
				return (R_FAIL);
			if(!wc_node)
			{
				if (befor)
					befor->next = after;
				if (after)
					after->prev = befor;
			}
			else
			{ 
				if (befor)
					befor->next = wc_node;
				else
					*args_list = wc_node;
				while (wc_node->next)
					wc_node = wc_node->next;
				if(after)
					after->prev = wc_node;
				wc_node->next = after;
			}
		}
	}
	return (R_SUCCESS);
}
