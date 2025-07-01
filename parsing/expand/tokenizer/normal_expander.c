/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 22:59:08 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/01 22:25:08 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parsing.h"

static int	calculate_data_len(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '$')
		i = 1;
	while (str[i] && !ft_isspace(str[i]) && \
		str[i] != '\'' && str[i] != '\"' && str[i] != '$')
		i++;
	return (i);
}

int	normal_expander(char *str, int *i, t_expand_token **tokens)
{
	int		len;
	char	*data;
	int		join;

	len = calculate_data_len(str);
	data = ft_substr(str, 0, len);
	if (!data)
		return (perror("error: "), R_FAIL);
	join = 1;
	if (ft_isspace(str[len]) || !str[len])
		join = 0;
	if (append_expand_token(tokens, data, 1, join, 0) == R_FAIL)
		return (R_FAIL);
	*i += len;
	return (R_SUCCESS);
}
