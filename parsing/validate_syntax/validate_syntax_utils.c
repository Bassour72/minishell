/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:21:43 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/08 15:02:48 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

int	is_opperator_syntax(t_type type)
{
	return (type == OP_AND || type == OP_OR || type == PIPE);
}

void	put_operator_syntax_error_msg(t_type type)
{
	if (type == PIPE)
		ft_putendl_fd("syntax error near unexpected token `|\'", 2);
	else if (type == OP_AND)
		ft_putendl_fd("syntax error near unexpected token `&&\'", 2);
	else if (type == OP_OR)
		ft_putendl_fd("syntax error near unexpected token `||\'", 2);
}

void	put_redirections_syntax_error_msg(t_type type)
{
	if (type == RED_APPEND)
		ft_putendl_fd("syntax error near unexpected token `>>\'", 2);
	else if (type == RED_TRUNK)
		ft_putendl_fd("syntax error near unexpected token `>\'", 2);
	else if (type == HER_DOC)
		ft_putendl_fd("syntax error near unexpected token `<<\'", 2);
	else if (type == RED_INPUT)
		ft_putendl_fd("syntax error near unexpected token `<\'", 2);
}

static int	check_after_parenths(t_token *tokens, int i)
{
	t_type	type;

	while (tokens[++i].data)
	{
		type = tokens[i].type;
		if (type == WORD)
		{
			ft_putstr_fd("syntax error near unexpected token ", 2);
			ft_putstr_fd(tokens[i].data, 2);
			ft_putendl_fd("\'", 2);
			return (R_FAIL);
		}
		else if (type != HER_DOC && type != RED_APPEND && type != RED_INPUT && \
		type != RED_TRUNK && type != T_FILE_NAME)
			break ;
	}
	return (R_SUCCESS);
}

int	validate_single_parenth(t_token *tokens, int i, int *p)
{
	if (tokens[i].type == PAREN_OPEN || tokens[i].type == PAREN_CLOSE)
	{
		if (tokens[i].type == PAREN_OPEN)
		{
			if (i && !is_opperator_syntax(tokens[i - 1].type) && \
			tokens[i - 1].type != PAREN_OPEN)
				return (ft_putendl_fd("syntax error near unexpected token `(\'" \
				, 2), R_FAIL);
			*p += 1;
		}
		if (tokens[i].type == PAREN_CLOSE)
		{
			if (check_after_parenths(tokens, i) == R_FAIL)
				return (R_FAIL);
			if ((tokens[i + 1].data && tokens[i + 1].type == PAREN_OPEN) || \
			(i && tokens[i - 1].type == PAREN_OPEN))
				return (ft_putendl_fd("syntax error near unexpected token `)\'" \
				, 2), R_FAIL);
			*p -= 1;
		}
	}
	return (R_SUCCESS);
}
