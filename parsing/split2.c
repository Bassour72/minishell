
#include "../include/parsing.h"



static int append_node(t_node **list, char **data)
{
	t_node *node;

	// printf("new_node_data [%s]\n", *data);

	// printf("data : {%s}\n", *data);
	if (!*list)
	{
		// printf("new node\n");
		*list = malloc(sizeof(t_node));
		if (!*list)
			return (0);
		(*list)->data = ft_strdup(*data);
		if (!(*list)->data)
			return (0);
		(*list)->next = NULL;
		free(*data);
		*data = NULL;
		return (1);
	}
	else
	{
		t_node *tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		node = malloc(sizeof(t_node));
		if (!node)
			return (0);
		node->data = ft_strdup(*data);
		if (!node->data)
			return (0);
		node->next = NULL;
		tmp->next = node;
		free(*data);
		*data = NULL;
		return (1);
	}
}

static int	m_operators(t_node **list, char *str, int *i)
{
	char *data;
	int state;

	state = 1;
	data = NULL;
	if (*(str + *i) == '|' && *(str + *i + 1) == '|')
	{
		data = malloc(3);
		data[0] = data[1] = '|';
		data[2] = '\0';
		state = append_node(list, &data);
		(*i) += 2;
		
	}
	else if (*(str + *i) == '|')
	{
		data = malloc(2);
		data[0] = '|';
		data[1] = '\0';
		state = append_node(list, &data);
		(*i) += 1;
	}
	else if (*(str + *i) == '&')
	{
		data = malloc(3);
		data[0] = data[1] = '&';
		data[2] = '\0';
		state = append_node(list, &data);
		(*i) += 2;
	}
	return (state);
}
static int m_parenth(t_node **list, char *str, int *i)
{
	char	*data;
	int		state;

	state = 1;
	data =  NULL;
	if (*(str + *i) == '(')
	{
		data = malloc(2);
		data[0] = '(';
		data[1] = '\0';
		state = append_node(list, &data);
		(*i) += 1;
	}
	else if (*(str + *i) == ')')
	{
		data = malloc(2);
		data[0] = ')';
		data[1] = '\0';
		state = append_node(list, &data);
		(*i) += 1;
	}
	return (state);
}

static int	m_redirections(t_node **list, char *str, int *i)
{
	int state;
	char *data;

	state = 1;
	data = NULL;
	if (*(str + *i + 1) == '<')
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
	else if (*(str + *i + 1) == '>')
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

static int	m_quotes(t_node **list, char *str, char **data, int *i)
{
	int		state;
	char	*_data;
	char	qoute_type;
	int		_i;
	int		len;
	int		j;

	j = 0;
	len = 0;
	_i = *i;
	qoute_type = *(str + _i);
	// printf("QUOTE TYPE = [%c]\n", qoute_type);
	state = 1;	

	if (!*data)
	{
		// printf("New Quote data\n");
		len = 2;
		_i++;
		while (*(str + _i) && *(str + _i) != qoute_type)
		{
			len++;
			_i++;
		}
		_i -= len - 1;
		_data = malloc(len + 1);


		if (!_data) //todo !!!!
			return (0);
		ft_strlcpy(_data, str + _i, len + 1);
		// printf("_data_size = %d | str + i = %s _data = %s\n", len, str + _i, _data);

		*data = _data;
		printf(">>%s<<\n", _data);
		(*i) = _i + len;
		// printf("end data: %s\n", *data);
		// printf("{%i}\n\n\n", *i);
		// state = append_node(list, data);
	}
	else
	{
		// printf("Append Quote data\n");
		// printf("start data: %s | start i: %d\n", *data, *i);
		len  = ft_strlen(*data);
		_i++;
		j = 2;
		while (*(str + _i) && *(str + _i) != qoute_type)
		{
			j++;
			_i++;
		}
		_i -= j - 1;
		_data = malloc(len + j + 1);
		if (!_data)
			return (0);
		int r = -1;
	
		ft_strlcpy(_data, *data, len + 1);
		ft_strlcpy(_data + len, str + _i, j + 1);
		
		free(*data);
		*data = _data;
		printf(">>%s<<\n", _data);

		(*i) = _i + j;
		// printf("end data: %s\n", *data);
		// printf("{%i}\n\n \n", *i);
	}
	return (state);
}

static int m_normal(t_node **list,char *str, char **data, int *i)
{
	int		state;
	char	*_data;
	int		_i;
	int		len;
	int		j;

	j = 0;
	len = 0;
	_i = *i;
	state = 1;

	if (!*data)
	{
		// printf("New Normal data\n");
		while (*(str + _i) && *(str + _i) != ' ' && *(str + _i) != '\'' && *(str + _i) != '\"' && !is_special(str + _i))
		{
			len++;
			_i++;
		}
		_i -= len;
		// printf("normal len: %d _i = %d\n", len, _i);
		_data = malloc(len + 1);
		if (!_data)
			return (0);
		ft_strlcpy(_data, str + _i, len + 1);
		// printf("Normal Data: %s\n", _data);
		// while (j < len)
		// {
		// 	// printf("[%c]\n", *(str + _i));
		// 	*(_data + j) = *(str + _i);
		// 	_i++;
		// 	j++;
		// }
		// *(_data + j) = '\0';
		*data = _data;
		(*i) = _i + len;
		// state = append_node(list, data);
		// printf("str: [%s]\n", *data);
		// printf("i = %d\n", _i);
	}
	else
	{
		len = ft_strlen(*data);
		while (*(str + _i) && *(str + _i) != ' ' && *(str + _i) != '\'' && *(str + _i) != '\"' && !is_special(str + _i))
		{
			j++;
			_i++;
		}
		_i -= j;
		// printf("normal len: %d _i: %d j: %d\n", len, _i, j);
		// printf("Normal start Data: %s\n", *data);
		_data = malloc(len + j + 1);
		if (!_data)
			return (free(*data), 0);

		ft_strlcpy(_data, *data, len + 1);
		ft_strlcpy(_data + len, str + _i  , j + 1);
		// int r = -1;
		// while (*(*data + ++r))
		// 	*(_data + r) = *((*data) + r);
		// while (r < len + j)
		// {
		// 	*(_data + r) = *(str + _i);
		// 	r++;
		// 	_i++;
		// }
		// *(_data + r) = '\0';
		free(*data);
		*data = _data;
		(*i) = _i + j;	
	}
	return (state);
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
	// free(list);
}

static void skip_spaces(char *str, int *i)
{
	while (*(str + *i) && *(str + *i) == ' ')
		(*i)++;
}

t_node *split2(char *str)
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
			if (!m_quotes(&list, str, &data, &i))
				return (cleaner(list), NULL);
		// printf("data: %s\n", data);// printf("prismo %c\n", *(str + i));
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
			if (!m_normal(&list, str, &data, &i))
				return (cleaner(list), NULL);
			if (data && (is_special(str + i) || str[i] == ' ' || !str[i]) && !append_node(&list, &data))
				return (cleaner(list), NULL);
		}
		skip_spaces(str, &i);
	}
	return list;
}
