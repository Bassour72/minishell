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

int validate_sytax(t_token *token)
{
	int i;
	i = -1;

	if (!validate_open_parenths(token) || !validate_close_parenths(token))
		return (0);
	while (token[++i].data)
	{
		if (is_op(token[i].type) && ((token[i + 1].data && is_op(token[i + 1].type)) || !token[i + 1].data ))
			return (put_operator_syntax_error_msg(token[i].type), 0);
		else if (_is_red(token[i].type) && (!token[i + 1].data || token[i + 1].type != T_FILE_NAME))
			return (put_redirections_syntax_error_msg(token[i].type), 0);
		else if (token[i].type == WORD)
		{
			if (i > 0 && (token[i - 1].type == PAREN_CLOSE))
				return (printf("bash: syntax error near unexpected token `%s'\n", token[i].data), 0);
		}
	}
	return 1;
}
