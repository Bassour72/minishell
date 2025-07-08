/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_arg_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybassour <ybassour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 21:57:55 by ybassour          #+#    #+#             */
/*   Updated: 2025/07/08 18:41:53 by ybassour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execution.h"

static int	has_three_consecutive_dots(const char *arg)
{
	int	i;
	int	dot_count;

	i = 0;
	dot_count = 0;
	while (arg[i])
	{
		if (arg[i] == '.')
		{
			dot_count++;
			if (dot_count == 3)
			{
				return (display_error(ERR_DOT_DOT_DOT, arg, STATUS_ERROR));
			}
		}
		else
			dot_count = 0;
		i++;
	}
	return (0);
}

static int	has_three_consecutive_hyphens(const char *arg)
{
	int	i;
	int	hyphen_count;

	i = 0;
	hyphen_count = 0;
	while (arg[i])
	{
		if (arg[i] == '-')
		{
			hyphen_count++;
			if (hyphen_count == 3)
			{
				return (display_error(ERR_HYPHEN_HYPHEN_HYPHEN, arg, \
						STATUS_ERROR));
			}
		}
		else
			hyphen_count = 0;
		i++;
	}
	return (0);
}

static int	has_invalid_char_combination(const char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i + 1])
	{
		if (arg[i] == '.' && arg[i + 1] == '-')
		{
			return (display_error(ERR_DOT_HYPHEN, arg, STATUS_ERROR));
		}
		if (arg[i] == '-' && (arg[i + 1] == '.' || arg[i + 1] == '/'))
		{
			if (arg[i + 1] == '.')
				return (display_error(ERR_HYPHEN_DOT, arg, STATUS_ERROR));
			else
				return (display_error(ERR_HYPHEN_SLASH, arg, STATUS_ERROR));
		}
		i++;
	}
	return (0);
}

int	is_illegal_cd_arg(const char *arg)
{
	if (!arg)
		return (0);
	if (has_three_consecutive_dots(arg))
		return (1);
	if (has_three_consecutive_hyphens(arg))
		return (1);
	if (has_invalid_char_combination(arg))
		return (1);
	return (0);
}

int	check_argument(t_tree *root)
{
	if (root && root->data && root->data[0] && root->data[1] && root->data[2])
	{
		perror("cd: too many arguments\n");
		return (1);
	}
	return (0);
}
