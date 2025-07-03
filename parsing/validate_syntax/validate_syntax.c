#include "../../include/parsing.h"

static int is_op(t_type type)
{
	return (type == OP_AND || type == OP_OR || type == PIPE);
}

static int _is_red(t_type type)
{
	return (type == RED_APPEND || type == RED_INPUT || type == RED_TRUNK || type == HER_DOC);
}

static void put_operator_syntax_error_msg(t_type type)
{
	if (type == PIPE)
		printf("syntax error near unexpected token `|\'\n");
	else if (type == OP_AND)
		printf("syntax error near unexpected token `&&\'\n");
	else if (type == OP_OR)
		printf("syntax error near unexpected token `||\'\n");
}

static void put_redirections_syntax_error_msg(t_type type)
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

int	validate_ppp(t_token *tokens)
{
	int	i = -1;
	int	p = 0;


	while (tokens[++i].data)
	{
		if (tokens[i].type == PAREN_OPEN || tokens[i].type == PAREN_CLOSE)
		{
			if(tokens[i].type == PAREN_OPEN)
			{
			 	if (i && !is_op(tokens[i - 1].type) && tokens[i - 1].type != PAREN_OPEN)// cmd (
					return (printf("syntax error near unexpected token `(\'\n"), 0);
				p++;
			}
			if (tokens[i].type == PAREN_CLOSE)
			{
				if ((tokens[i + 1].data && tokens[i + 1].type == PAREN_OPEN) || (i && tokens[i - 1].type == PAREN_OPEN))
					return (printf("syntax error near unexpected token `)\'\n"), 0);
				p--;
			}
		}
	}
	if (p > 0)
		return (printf("syntax error near unexpected token `(\'\n"), R_FAIL);
	else if(p < 0)
		return (printf("syntax error near unexpected token `)\'\n"), R_FAIL);
	return (R_SUCCESS);
}

int validate_sytax(t_token *token)
{
	int i;
	i = -1;
	if (validate_ppp(token) == R_FAIL)
		return (R_FAIL);
	while (token[++i].data)
	{
		if (_is_red(token[i].type) && (!token[i + 1].data || token[i + 1].type != T_FILE_NAME))
			return (put_redirections_syntax_error_msg(token[i].type), 0);
		else if (token[i].type == WORD)
		{
			if (i > 0 && (token[i - 1].type == PAREN_CLOSE))
				return (printf("bash: syntax error near unexpected token `%s'\n", token[i].data), 0);
		}
		else if (is_op(token[i].type))
		{
			if (i == 0)
				return (put_operator_syntax_error_msg(token[i].type), 0);

			if (token[i - 1].type != WORD && token[i - 1].type != T_FILE_NAME && token[i - 1].type != PAREN_CLOSE)
				return (put_operator_syntax_error_msg(token[i].type), 0);

			if (!token[i + 1].data || (token[i + 1].type != WORD && !_is_red(token[i + 1].type) && token[i + 1].type != PAREN_OPEN))
				return (put_operator_syntax_error_msg(token[i].type), 0);
		}
	}
	return 1;
}
