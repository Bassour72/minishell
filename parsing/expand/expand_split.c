/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:29:38 by massrayb          #+#    #+#             */
/*   Updated: 2025/06/29 22:35:32 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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

static int	append_node_expand(t_expand_node **store, char *data)
{
	t_expand_node	*new_node;
	t_expand_node	*tmp;

	new_node = malloc(sizeof(t_expand_node));
	if (!new_node)
		return (perror("error: "), R_FAIL);
	new_node->data = data;
	new_node->joinable = 0;
	new_node->next = NULL;
	if (!*store)
		*store = new_node;
	else
	{
		tmp = *store;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (R_SUCCESS);
}

static int	do_split(t_expand_node **sub_list, char *str, char split_char)
{
	int				start;
	char			*data;
	int				i;

	i = 0;
	while (str[i])
	{
		while (str[i] == split_char)
			i++;
		start = i;
		while (str[i] && str[i] != split_char)
			i++;
		data = custom_strdub(str + start, i - start);
		if (data == NULL)
		{
			perror("error: ");
			return (free_expand_list_nodes(*sub_list), R_FAIL);
		}
		if (append_node_expand(sub_list, data) == R_FAIL)
		{
			free_expand_list_nodes(*sub_list);
			return (R_FAIL);
		}
	}
	return (R_SUCCESS);
}

int	expand_split(t_expand_node **sub_list, char const *str, char c, int join)
{
	t_expand_node	*tmp;

	if (do_split(sub_list, (char *)str, c) == R_FAIL)
		return (R_FAIL);
	if (join)
	{
		tmp = *sub_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->joinable = join;
	}
	return (R_SUCCESS);
}
