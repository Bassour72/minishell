
#include "../../include/parsing.h"

int is_special(char *c)
{
	return (*c == '(' || *c == ')' || *c == '<' || *c == '>' || *c == '|' || *c == '&');
}


static void cleaner(t_node *list)
{
	t_node *_list;
	t_node *tmp;

	_list = list;
	while (_list)
	{
		tmp = _list;
		_list = _list->next;
		free(tmp->data);
		free(tmp);
	}
}

static void skip_spaces(char *str, int *i)
{
	while (*(str + *i) && *(str + *i) == ' ')
		(*i)++;
}

int quotes(char *str, char **data, t_node **list, int *i)
{
	if (!m_quotes(str, data, i))
		return (cleaner(*list), 0);
	if (*data && (str[*i] == ' ' || is_special(str + *i) || !str[*i]) && !append_node(list, data))
		return (cleaner(*list), 0);
	return (1);
}
int specials(char *str, char **data, t_node **list, int *i)
{
	if (*data && !append_node(list, data))
		return (cleaner(*list), 0);
	if (!m_operators(list, str, i))
		return (cleaner(*list), 0);
	if (!m_parenth(list, str, i))
		return (cleaner(*list), 0);
	if (!m_redirections(list, str, i))
		return (cleaner(*list), 0);
	return (1);
}

int normal(char *str, char **data, t_node **list, int *i)
{
	if (!m_normal(str, data, i))
		return (cleaner(*list), 0);
	if (*data && (is_special(str + *i) || str[*i] == ' ' || !str[*i]) && !append_node(list, data))
		return (cleaner(*list), 0);
	return (1);
}

t_node *split(char *str)
{
	t_node	*list;
	char 	*data;
	int		i;

	list = NULL;
	data = NULL;
	i = 0;
	if (!str)
		return (NULL);
	while (*(str + i) != '\0')
	{
		skip_spaces(str, &i);
		
		if (*(str + i) == '\"' || *(str + i) == '\'')
			quotes(str, &data, &list, &i);
		else if (is_special(str + i))
			specials(str, &data, &list, &i);
		else
			normal(str, &data, &list, &i);
		skip_spaces(str, &i);
	}
	return list;
}
