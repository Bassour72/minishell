/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:10:43 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 15:23:01 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	_is_red(t_type type)
{
	return (type == RED_APPEND || type == RED_INPUT || type == RED_TRUNK || \
	type == HER_DOC);
}

int	validate_parenths(t_token *tokens)
{
	int	i;
	int	p;

	p = 0;
	i = -1;
	while (tokens[++i].data)
	{
		if (validate_single_parenth(tokens, i, &p) == R_FAIL)
			return (R_FAIL);
	}
	if (p > 0)
		return (ft_putendl_fd("syntax error near unexpected token `(\'", 2), 0);
	else if (p < 0)
		return (ft_putendl_fd("syntax error near unexpected token `)\'", 2), 0);
	return (R_SUCCESS);
}

static int	validate_opperator(t_token *token, int i)
{
	if (i == 0)
		return (put_operator_syntax_error_msg(token[i].type), 0);
	if (token[i - 1].type != WORD && token[i - 1].type != \
	T_FILE_NAME && token[i - 1].type != PAREN_CLOSE)
		return (put_operator_syntax_error_msg(token[i].type), 0);
	if (!token[i + 1].data || (token[i + 1].type != WORD && \
	!_is_red(token[i + 1].type) && token[i + 1].type != PAREN_OPEN))
		return (put_operator_syntax_error_msg(token[i].type), 0);
	return (R_SUCCESS);
}

static void	put_error(char *str)
{
	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("\'", 2);
}

int	validate_sytax(t_token *token)
{
	int	i;

	i = -1;
	if (validate_parenths(token) == R_FAIL)
		return (R_FAIL);
	while (token[++i].data)
	{
		if (_is_red(token[i].type) && (!token[i + 1].data || \
		token[i + 1].type != T_FILE_NAME))
			return (put_redirections_syntax_error_msg(token[i].type), 0);
		else if (token[i].type == WORD)
		{
			if (i > 0 && (token[i - 1].type == PAREN_CLOSE))
				return (put_error(token[i].data), R_FAIL);
		}
		else if (is_opperator_syntax(token[i].type))
		{
			if (validate_opperator(token, i) == R_FAIL)
				return (R_FAIL);
		}
	}
	return (R_SUCCESS);
}
