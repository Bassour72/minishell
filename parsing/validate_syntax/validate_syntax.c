#include "../../include/parsing.h"

int is_op(t_type type)
{
	return (type == OP_AND || type == OP_OR || type == PIPE);
}

static int _is_red(t_type type)
{
	return (type == RED_APPEND || type == RED_INPUT || type == RED_TRUNK || type == HER_DOC);
}

int validate_parenths(t_token *token, int i)
{
	int p;

	p = 0;
	if (token[i + 1].data && token[i + 1].type == PAREN_CLOSE)
		return (printf("syntax error near unixpected token `)\'\n"), 0);
	if (i != 0)
		if (!is_op(token[i - 1].type))
			return (printf("syntax error: befor (\n"), 0);
	while (token[i].data)
	{
		if (token[i].type == PAREN_OPEN)
			p++;
		else if (token[i].type == PAREN_CLOSE)
			p--;
		i++;
	}
	if (p > 0)
		return (printf("syntax error near unexpected token `(\'\n"), 0);
	else if (p < 0)
		return (printf("syntax error near unexpected token `)\'\n"), 0);
	return (1);
}

int validate_operator(t_token *token, int i)
{
	if (i == 0 || is_op(token[i].type))
		return (printf("syntax error near unexpected token `|\'\n"), 0);
	if (token[i + 1].data || is_op(token[i + 1].type))
		return (printf("syntax error near unexpected token `|\'\n"), 0);
	return (1);
}

int validate_quotes(char *str)
{
	int i;
	char quote_type;

	quote_type = '\0';
	i = -1;
	while (str[++i])
	{
		if(quote_type == '\0' && (str[i] == '\'' || str[i] == '\"'))
			quote_type = str[i];
		else if (quote_type == str[i])
			quote_type = '\0';
	}
	if (quote_type != '\0')
	{
		if (quote_type == '\"')
			return (printf("syntax error near unexpected token `\"\n"), 0);
		else if (quote_type == '\'')
			return (printf("syntax error near unexpected token `\'\n"), 0);
	}
	return (1);
}


int validate_sytax(t_token *token)
{
	int i;
	i = -1;
	while (token[++i].data)
	{
		if (is_op(token[i].type) && ((token[i + 1].data && is_op(token[i + 1].type)) || !token[i + 1].data ))
		{
			if (token[i].type == PIPE)
				return (printf("syntax error near unexpected token `|\'\n"), 0);
			else if (token[i].type == OP_AND)
				return (printf("syntax error near unexpected token `&&\'\n"), 0);
			else if (token[i].type == OP_OR)
				return (printf("syntax error near unexpected token `||\'\n"), 0);
		}
		if (_is_red(token[i].type))
			if (token[i + 1].type != T_FILE_NAME)
			{
				if (token[i].type == RED_APPEND)
					return (printf("syntax error near unexpected token `>>\'\n"), 0);
				else if (token[i].type == RED_TRUNK)
					return (printf("syntax error near unexpected token `>\'\n"), 0);
				else if (token[i].type == HER_DOC)
					return (printf("syntax error near unexpected token `<<\'\n"), 0);
				else if (token[i].type == RED_INPUT)
					return (printf("syntax error near unexpected token `<\'\n"), 0);

			}
		if (token[i].type == PAREN_OPEN)
			if (!validate_parenths(token, i))
				return (0);
	}
	return 1;
}
