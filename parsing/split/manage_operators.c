
#include "../../include/parsing.h"

static int _OR(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (0);
	data[0] = '|';
	data[1] = '|';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

static int _PIPE(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(2);
	if (!data)
		return (0);
	data[0] = '|';
	data[1] = '\0';
	state = append_node(list, &data);
	(*i) += 1;
	return (state);
}

static int _AND(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	data = malloc(3);
	if (!data)
		return (0);
	data[0] = '&';
	data[1] = '&';
	data[2] = '\0';
	state = append_node(list, &data);
	(*i) += 2;
	return (state);
}

int	m_operators(t_node **list, char *str, int *i)
{
	char *data;
	int state;

	state = 1;
	data = NULL;
	if (*(str + *i) == '|' && *(str + *i + 1) == '|')
		state = _OR(list, str, i);
	else if (*(str + *i) == '|')
		state = _PIPE(list, str, i);
	else if (*(str + *i) == '&')
		state = _AND(list, str, i);
	return (state);
}