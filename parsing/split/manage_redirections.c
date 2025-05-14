#include "../../include/parsing.h"

int	m_redirections(t_node **list, char *str, int *i)
{
	int state;
	char *data;

	state = 1;
	data = NULL;
	if (*(str + *i) == '<' && *(str + *i + 1) == '<')
	{
		data = malloc(3);
		data[0] = data[1] = '<';
		data[2] = '\0';
		state = append_node(list, &data);
		(*i) += 2;
	}
	else if (*(str + *i) == '<')
	{
		data = malloc(2);
		data[0] = '<';
		data[1] = '\0';
		state = append_node(list, &data);
		(*i) += 1;
	}
	else if (*(str + *i) == '>' && *(str + *i + 1) == '>')
	{
		data = malloc(3);
		data[0] = data[1] = '>';
		data[2] = '\0';
		state = append_node(list, &data);
		(*i) += 2;
	}
	else if (*(str + *i) == '>')
	{
		data = malloc(2);
		data[0] = '>';
		data[1] = '\0';
		state = append_node(list, &data);
		(*i) += 1;
	}
	return (state);
}
