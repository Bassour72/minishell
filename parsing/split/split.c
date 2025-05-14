
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

t_node *split(char *str)
{
	t_node	*list = NULL;
	char 	*data = NULL;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	while (*(str + i) != '\0')
	{
		skip_spaces(str, &i);
		
		if (*(str + i) == '\"' || *(str + i) == '\'')
		{
			if (!m_quotes(str, &data, &i))
				return (cleaner(list), NULL);
			if (data && (str[i] == ' ' || is_special(str + i) || !str[i]) && !append_node(&list, &data))
				return (cleaner(list), NULL);
		}
		else if (is_special(str + i))
		{
			if (data && !append_node(&list, &data))
				return (cleaner(list), NULL);
			if (!m_operators(&list, str, &i))
				return (cleaner(list), NULL);
			if (!m_parenth(&list, str, &i))
				return (cleaner(list), NULL);
			if (!m_redirections(&list, str, &i))
				return (cleaner(list), NULL);
		}
		else
		{
			if (!m_normal(str, &data, &i))
				return (cleaner(list), NULL);
			if (data && (is_special(str + i) || str[i] == ' ' || !str[i]) && !append_node(&list, &data))
				return (cleaner(list), NULL);
		}
		skip_spaces(str, &i);
	}
	return list;
}
