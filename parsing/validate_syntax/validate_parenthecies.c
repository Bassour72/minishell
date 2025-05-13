#include "../../include/parsing.h"

static int is_op(t_type type)
{
	return (type == OP_AND || type == OP_OR || type == PIPE);
}

static int _is_red(t_type type)
{
	return (type == RED_APPEND || type == RED_INPUT || type == RED_TRUNK || type == HER_DOC);
}

static void update_p(int *p, t_type type)
{
	if (type == PAREN_OPEN)
		(*p)++;
	else if (type == PAREN_CLOSE)
		(*p)--;
}

int check_front(t_token *token, int i)
{
	if (token[i + 1].data && (!is_op(token[i + 1].type) && token[i + 1].type != PAREN_CLOSE && !_is_red(token[i + 1].type)))
		return (printf("syntax error near unexpected token `)\'\n"), 0);
	return (1);
}

int validate_close_parenths(t_token *token)
{
	int i;
	int p;
	int j;

	i = -1;
	while (token[++i].data)
		;
	while (--i >= 0)
	{
		if (token[i].type == PAREN_CLOSE)
		{
			if (!check_front(token, i))
				return (0);
			p = 0;
			j = i + 1;
			while (--j >= 0)
			{
				update_p(&p, token[j].type);
				if (p == 0)
					break;
			}
			if (p != 0)
				return (printf("syntax error near unexpected token `)\'\n"), 0);
		}
	}
	return (1);
}

int check_back(t_token *token, int i)
{
	if (i > 0 && (!is_op(token[i - 1].type) && token[i - 1].type != PAREN_OPEN))
		return (printf("syntax error near unexpected token `(\'\n"), 0);
	return (1);
}



int validate_open_parenths(t_token *token)
{
	int p;
	int	i;
	int j;

	i = -1;
	while (token[++i].data)
	{
		if (token[i].type == PAREN_OPEN)
		{
			if (!check_back(token, i))
				return (0);
			p = 0;
			j = i;
			while (token[++j].data)
			{
				update_p(&p, token[j].type);
				if (p == 0)
					break;
			}
			if (p != 0)
				return (printf("syntax error near unexpected token `(\'\n"), 0);
		}
	}
	return (1);
}
