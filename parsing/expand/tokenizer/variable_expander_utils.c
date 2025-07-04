/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expander_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 23:14:56 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/05 00:29:09 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

int	is_valid_key_char(char c, int i)
{
	if (i == 0)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			return (1);
		else
			return (0);
	}
	else
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		c == '_' || (c >= '0' && c <= '9'))
			return (1);
		else
			return (0);
	}
}

int	extarct_var_key(char *str, char **key)
{
	int	len;

	len = 0;
	if (str[0] == '?')
	{
		*key = ft_strdup("exit_status@gmail.com");
		if (!*key)
			return (perror("error: "), R_FAIL);
		return (R_SUCCESS);
	}
	while (str[len] && is_valid_key_char(str[len], len))
		len++;
	if (len == 0)
		return (*key = NULL, R_SUCCESS);
	*key = ft_substr(str, 0, len);
	if (!*key)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

static void	check_and_replace(char *value)
{
	if (!value)
		return ;
	while (*value)
	{
		if (*value == 14)
			*value = 13;
		else if (*value == 15)
			*value = 16;
		value++;
	}
}

static void	update_value(char *value)
{
	if (!value)
		return ;
	while (*value)
	{
		if (*value == '\"')
			*value = DOUBLE_QUOTE;
		else if (*value == '\'')
			*value = SINGLE_QUOTE;
		value++;
	}
}

int	extract_var_value(char *src, int *i, char **dst, t_env *env)
{
	char	*key;
	char	*value;

	if (extarct_var_key(src + 1, &key) == R_FAIL)
		return (R_FAIL);
	if (!key)
		return (*dst = NULL, R_SUCCESS);
	value = env_get_value(env, key);
	update_value(value);
	if (ft_strcmp("exit_status@gmail.com", key) == 0)
		(*i) += 2;
	else
		(*i) += ft_strlen(key) + 1;
	free(key);
	if (value)
	{
		*dst = ft_strdup(value);
		if (!*dst)
			return (perror("error: "), R_FAIL);
	}
	else
	{
		*dst = calloc(1, 1);
	}
	return (R_SUCCESS);
}
