/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_herdoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:16:36 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/06 19:12:40 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	join_line(char **str1, char *str2)
{
	char	*new_str;

	new_str = *str1;
	new_str = ft_strjoin(*str1, str2);
	free(*str1);
	free(str2);
	if (!new_str)
		return (perror("error: "), R_FAIL);
	*str1 = new_str;
	return (R_SUCCESS);
}

static int	expand_variable(char *line, int *i, char **result, t_env *env)
{
	char	*value;

	value = NULL;
	if (is_valid_key_char(line[(*i) + 1], 0) || line[(*i) + 1] == '?')
	{
		if (extract_var_value(line + *i, i, &value, env) == R_FAIL)
			return (free(*result), R_FAIL);
		if (value && join_line(result, value) == R_FAIL)
			return (R_FAIL);
	}
	else if (line [(*i) + 1] && line[(*i) + 1] != '\'' \
	&& line[(*i) + 1] != '\"')
		(*i) += 2;
	else
		(*i) += 1;
	return (R_SUCCESS);
}

static int	save_normal(char *line, int *i, char **result)
{
	int		len;
	char	*value;

	value = NULL;
	len = (*i) + 1;
	while (line[len] && line[len] != '$')
		len++;
	value = ft_substr(line, *i, len - *i);
	if (!value)
		return (perror("error: "), free(*result), R_FAIL);
	if (join_line(result, value) == R_FAIL)
		return (R_FAIL);
	*i = len;
	return (R_SUCCESS);
}

static int	finalize_herdoc_expander(char **herdoc_line, char *result)
{
	free(*herdoc_line);
	*herdoc_line = result;
	if (!*herdoc_line)
	{
		*herdoc_line = calloc(1, 1);
		if (!*herdoc_line)
			return (R_FAIL);
	}
	return (R_SUCCESS);
}

int	expand_herdoc(char **herdoc_line, t_env *env)
{
	char	*line;
	char	*result;
	int		len;
	int		i;

	result = NULL;
	line = *herdoc_line;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0' && !ft_isspace(line[i + 1]))
		{
			if (expand_variable(line, &i, &result, env) == R_FAIL)
				return (R_FAIL);
		}
		else
		{
			if (save_normal(line, &i, &result) == R_FAIL)
				return (R_FAIL);
		}
	}
	return (finalize_herdoc_expander(herdoc_line, result));
}
