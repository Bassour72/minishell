/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:21:43 by massrayb          #+#    #+#             */
/*   Updated: 2025/07/07 14:55:54 by massrayb         ###   ########.fr       */
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
		printf("syntax error near unexpected token `|\'\n");
	else if (type == OP_AND)
		printf("syntax error near unexpected token `&&\'\n");
	else if (type == OP_OR)
		printf("syntax error near unexpected token `||\'\n");
}

void	put_redirections_syntax_error_msg(t_type type)
{
	if (type == RED_APPEND)
		printf("syntax error near unexpected token `>>\'\n");
	else if (type == RED_TRUNK)
		printf("syntax error near unexpected token `>\'\n");
	else if (type == HER_DOC)
		printf("syntax error near unexpected token `<<\'\n");
	else if (type == RED_INPUT)
		printf("syntax error near unexpected token `<\'\n");
}

static int	check_after_parenths(t_token *tokens, int i)
{
	t_type	type;

	while (tokens[++i].data)
	{
		type = tokens[i].type;
		if (type == WORD)
			return (printf("syntax error near unexpected token `%s\'\n", \
			tokens[i].data), 0);
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
				return (printf("syntax error near unexpected token `(\'\n"), 0);
			*p += 1;
		}
		if (tokens[i].type == PAREN_CLOSE)
		{
			if (check_after_parenths(tokens, i) == R_FAIL)
				return (R_FAIL);
			if ((tokens[i + 1].data && tokens[i + 1].type == PAREN_OPEN) || \
			(i && tokens[i - 1].type == PAREN_OPEN))
				return (printf("syntax error near unexpected token `)\'\n"), 0);
			*p -= 1;
		}
	}
	return (R_SUCCESS);
}
