/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_file_names.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:46:09 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 14:54:08 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static void	change_quotes(char *str)
{
	while (*str)
	{
		if (*str == '\"')
			*str = DOUBLE_QUOTE;
		else if (*str == '\'')
			*str = SINGLE_QUOTE;
		str++;
	}
}

static void	init_new_node(t_wc_node *node, char *new_data)
{
	node->data = new_data;
	node->private = 0;
	if (new_data[0] == '.')
		node->private = 1;
	node->match = 0;
	node->next = NULL;
	node->prev = NULL;
}

int	append_wc_node(t_wc_node **list, char *data, int flag)
{
	t_wc_node	*node;
	t_wc_node	*tmp;
	char		*new_data;

	new_data = ft_strdup(data);
	if (!new_data)
		return (perror("error: "), R_FAIL);
	if (flag)
		change_quotes(new_data);
	node = malloc(sizeof(t_wc_node));
	if (!node)
		return (perror("error: "), free(new_data), R_FAIL);
	init_new_node(node, new_data);
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
	return (R_SUCCESS);
}

int	init_file_names(t_wc_node **file_names)
{
	char			*cwd;
	DIR				*dir;
	struct dirent	*dir_entry;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (R_CONTINUE);
	dir = opendir(cwd);
	if (!dir)
		return (free(cwd), R_CONTINUE);
	errno = 0;
	while (errno == 0)
	{
		dir_entry = readdir(dir);
		if (!dir_entry)
			break ;
		if (append_wc_node(file_names, dir_entry->d_name, 1) == R_FAIL)
			return (free_wc_node_list(*file_names), \
			closedir(dir), free(cwd), R_FAIL);
	}
	if (dir)
		closedir(dir);
	if (errno != 0)
		return (free_wc_node_list(*file_names), free(cwd), R_CONTINUE);
	return (free(cwd), R_SUCCESS);
}
