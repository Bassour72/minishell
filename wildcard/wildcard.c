/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 23:55:55 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 22:17:44 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static int	init_args_list(t_wc_node **list, char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (append_wc_node(list, args[i], 0) == R_FAIL)
			return (free_wc_node_list(*list), R_FAIL);
	}
	return (R_SUCCESS);
}

static int	init_filenames_and_argslist(char ***args, t_wc_node **file_names, \
t_wc_node **args_list)
{
	int	state;

	if (!args)
		return (R_SUCCESS);
	1 && (*file_names = NULL, *args_list = NULL);
	state = init_file_names(file_names);
	if (state == R_CONTINUE)
		return (R_CONTINUE);
	else if (state == R_FAIL)
		return (R_FAIL);
	*file_names = sort_filenames(*file_names);
	if (init_args_list(args_list, *args) == R_FAIL)
		return (free_2d_arr(*args), free_wc_node_list(*file_names), R_FAIL);
	free_2d_arr(*args);
	return (R_SUCCESS);
}

int	wildcard(char ***args)
{
	t_wc_node	*file_names;
	t_wc_node	*args_list;
	int			len;
	int			state;

	state = init_filenames_and_argslist(args, &file_names, &args_list);
	if (state == R_CONTINUE)
		return (R_CONTINUE);
	else if (state == R_FAIL)
		return (R_FAIL);
	if (expand_wildcard(&args_list, file_names) == R_FAIL)
	{
		(free_wc_node_list(file_names), free_wc_node_list(args_list));
		return (R_FAIL);
	}
	free_wc_node_list(file_names);
	len = get_wc_list_len(args_list);
	*args = malloc(sizeof(char *) * (len + 1));
	if (!*args)
		return (perror("error: "), free_wc_node_list(args_list), R_FAIL);
	assign_wc_node_data_to_args_array(args_list, args);
	cleanup_wc_list_shels(args_list);
	return (R_SUCCESS);
}
