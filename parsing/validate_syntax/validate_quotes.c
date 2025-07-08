/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:13:05 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 15:01:11 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	validate_quotes(char *str)
{
	int		i;
	char	quote_type;

	quote_type = '\0';
	i = -1;
	while (str[++i])
	{
		if (quote_type == '\0' && (str[i] == '\'' || str[i] == '\"'))
			quote_type = str[i];
		else if (quote_type == str[i])
			quote_type = '\0';
	}
	if (quote_type != '\0')
	{
		if (quote_type == '\"')
			return (ft_putendl_fd("syntax error near unexpected token `\"", 2), \
			R_FAIL);
		else if (quote_type == '\'')
			return (ft_putendl_fd("syntax error near unexpected token `\'", 2), \
			R_FAIL);
	}
	return (1);
}
