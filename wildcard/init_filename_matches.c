/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_filename_matches.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:30:23 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 22:54:25 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wildcard.h"

static void	jump_the_quotes_after_star(t_is_match_args *args, char c)
{
	args->i += 1;
	args->wc_pos = args->i;
	if (c == '\'' || c == '\"')
		args->wc_pos = ++args->i + 1;
}

static int	reset_i_to_after_star_and_confirm_missmatch(t_is_match_args *args)
{
	if (args->wc_pos != args->i)
		args->i = args->wc_pos;
	else
		args->j++;
	if ((args->j == args->file_name_len && args->wc_pos == args->i \
	&& args->i != args->str_len) || args->wc_pos == -1)
		return (0);
	else if (args->j == args->file_name_len && args->i == args->str_len)
		return (1);
	return (2);
}

static t_is_match_args	init_is_match_args(char *file_name, char *str)
{
	t_is_match_args	args;

	args.i = 0;
	args.j = 0;
	args.is_literal_string = -1;
	args.wc_pos = -1;
	args.file_name_len = ft_strlen(file_name);
	args.str_len = ft_strlen(str);
	return (args);
}

int	is_file_name_match(char *file_name, char *str)
{
	t_is_match_args	args;
	int				state;

	args = init_is_match_args(file_name, str);
	while (1)
	{
		if (str[args.i] == '\"' || str[args.i] == '\'')
			1 && (args.i++, args.wc_pos++, args.is_literal_string = 1);
		else if (str[args.i] == '*' && args.is_literal_string == -1)
			jump_the_quotes_after_star(&args, str[args.i]);
		else if (str[args.i] != file_name[args.j])
		{
			state = reset_i_to_after_star_and_confirm_missmatch(&args);
			if (state == 0 || state == 1)
				return (state);
		}
		else
		{
			if (args.i == args.str_len && args.j == args.file_name_len)
				return (1);
			1 && (args.i++, args.j++);
		}
		if (args.j == args.file_name_len && args.i == args.str_len)
			return (1);
	}
}

void	init_file_names_matches(t_wc_node *file_names, char *name)
{
	while (file_names)
	{
		if (is_file_name_match(file_names->data, name) == 1)
		{
			file_names->match = 1;
			if (file_names->data[0] == '.' && name[0] != '.')
				file_names->match = 0;
		}
		file_names = file_names->next;
	}
}
