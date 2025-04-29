
// #include <stdio.h>
// #include <stdlib.h>
// #include <libc.h>

#include "../include/parsing.h"

// typedef struct s_node
// {
// 	char *data;
// 	struct s_node *next;
// }	t_node;

int is_special(char *c)
{
	return (*c == '(' || *c == ')' || *c == '<' || *c == '>' || *c == '|' || *c == '&');
}

void *append_node(t_node **list, char **data)
{
	t_node *node;

	printf("data : {%s}\n", *data);

	if (!*list)
	{
		// printf("new node\n");
		*list = malloc(sizeof(t_node));
		if (!*list)
			return (NULL);
		(*list)->data = ft_strdup(*data);
		(*list)->next = NULL;
		*data = NULL;
		return (data);

	}
	else
	{
		// printf("append node\n");
		t_node *tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		node = malloc(sizeof(t_node));
		if (!node)
			return (NULL);
		node->data = ft_strdup(*data);
		node->next = NULL;
		tmp->next = node;
		*data = NULL;
		return (node);
	}
}



void m_redirections(t_node **list, char *str, int *i)
{
	char *data;

	data = NULL;
	if (*(str + *i + 1) == '<')
	{
		data = malloc(3);
		data[0] = data[1] = '<';
		data[2] = '\0';
		append_node(list, &data);
		(*i) += 2;
	}
	else if (*(str + *i) == '<')
	{
		data = malloc(2);
		data[0] = '<';
		data[1] = '\0';
		append_node(list, &data);
		(*i) += 1;
	}
	else if (*(str + *i + 1) == '>')
	{
		data = malloc(3);
		data[0] = data[1] = '>';
		data[2] = '\0';
		append_node(list, &data);
		(*i) += 2;
	}
	else if (*(str + *i) == '>')
	{
		data = malloc(2);
		data[0] = '>';
		data[1] = '\0';
		append_node(list, &data);
		(*i) += 1;
	}
}
void m_operators(t_node **list, char *str, int *i)
{
	char *data;

	data = NULL;
	if (*(str + *i) == '|' && *(str + *i + 1) == '|')
	{
		data = malloc(3);
		data[0] = data[1] = '|';
		data[2] = '\0';
		append_node(list, &data);
		(*i) += 2;
	}
	else if (*(str + *i) == '|')
	{
		data = malloc(2);
		data[0] = '|';
		data[1] = '\0';
		append_node(list, &data);
		(*i) += 1;
	}
	else if (*(str + *i) == '&')
	{
		data = malloc(3);
		data[0] = data[1] = '&';
		data[2] = '\0';
		append_node(list, &data);
		(*i) += 2;
	}
}
void m_parenth(t_node **list, char *str, int *i)
{
	char *data;

	data =  NULL;

		if (*(str + *i) == '(')
		{
			data = malloc(2);
			data[0] = '(';
			data[1] = '\0';
			append_node(list, &data);
			(*i) += 1;
		}
		else if (*(str + *i) == ')')
		{
			data = malloc(2);
			data[0] = ')';
			data[1] = '\0';
			append_node(list, &data);
			(*i) += 1;
		}

}

void m_quotes(t_node **list, char *str, char **data, int *i)
{
	char	*_data;
	char	q;
	int		_i;
	int		len;
	int		j;

	j = 0;
	len = 0;
	_i = *i;
	q = *(str + _i);
	

	if (!*data)
	{
		len = 2;
		_i++;
		while (*(str + _i) && *(str + _i) != q)
		{
			len++;
			_i++;
		}
		_i -= len - 1;
		printf("%d\n", _i);
		_data = malloc(len + 1);
		while (j < len)
		{
			*(_data + j) = *(str + _i);
			_i++;
			j++;
		}
		*(_data + j) = '\0';
		*data = _data;
		(*i) = _i;
		append_node(list, data);
	}
	else
	{
		len  = ft_strlen(*data);
		while (*(str + _i) && *(str + _i) != q)
		{
			j++;
			_i++;
		}
		_i -= j;
		_data = malloc(len + j + 1);
		int r = -1;
		while ((*data) + ++r)
			*(_data + r) = *(*(data) + r);

		while( r < len + j)
		{
			*(_data + r) = *(str + _i);
			_i++;
			r++;
		}
		*(_data + r) = '\0';
		free(*data);
		*data = _data;
		(*i) = _i;
	}
}

void m_normal(t_node **list,char *str, char **data, int *i)
{
	char *_data;
	int _i;
	int len;
	int j;

	j = 0;
	len = 0;
	_i = *i;
	// while (*(str + _i) && *(str + _i) == ' ')
	// 	_i++;
	// if (*(str + _i) == '\0')
	// {
	// 	*i = _i;
	// 	return;
	// }
	if (!*data)
	{
		while (*(str + _i) && *(str + _i) != ' ' && *(str + _i) != '\'' && *(str + _i) != '\"' && !is_special(str + _i))
		{
			len++;
			_i++;
		}
		_i -= len;
		// printf("normal len: %d\n", len);
		_data = malloc(len + 1);
		while (j < len)
		{
			// printf("[%c]\n", *(str + _i));
			*(_data + j) = *(str + _i);
			_i++;
			j++;
		}
		*(_data + j) = '\0';
		*data = _data;
		(*i) = _i;
		append_node(list, data);
		// printf("str: [%s]\n", *data);
		printf("i = %d\n", _i);
	}
	else
	{
		len = ft_strlen(*data);
		while (*(str + _i) && *(str + _i) != ' ' && *(str + _i) != '\'' && *(str + _i) != '\"' && !is_special(str + _i))
		{
			j++;
			_i++;
		}
		_i -= len;
		_data = malloc(len + j + 1);
		int r = -1;
		while (*(*data + ++r))
			*(_data + r) = *((*data) + r);
		while (r < len + j)
		{
			*(_data + r) = *(str + _i);
			r++;
			_i++;
		}
		*(_data + r) = '\0';
		free(*data);
		*data = _data;
		(*i) = _i;
	}
}


t_node *split(char *str)
{
	t_node *list = NULL;

	char *data = NULL;

	int i = 0;
	int j = 0;
	while (*(str + i) != '\0')
	{
		while (*(str + i) && *(str + i) == ' ')
			i++;
		printf("prismo %c\n", *(str + i));
		if (*(str + i) == '\"' || *(str + i) == '\'')
		{
			if(data)
			{
				append_node(&list, &data);
			}
			m_quotes(&list, str, &data, &i);
		}
		else if (is_special(str + i))
		{
			if (data)
				append_node(&list, &data);
			m_operators(&list, str, &i);
			m_parenth(&list, str, &i);
			m_redirections(&list, str, &i);
		}
		else //if (*(str + i) != ' ')
			m_normal(&list, str, &data, &i);
		

		while (*(str + i) && *(str + i) == ' ')
			i++;
		printf("main i: %d\n", i);
		// else 
		// 	i++;

		// printf("str + i : <%c>\n", *(str + i));
		// sleep(1);
	}
	return list;
}

// int main(int ac, char **av)
// {
	
// 	t_node *list = split("\"ab\"> \" |   \"");

// 	printf("\n ==============\n");

// 	int i = -1;
// 	while (list)
// 	{
// 		printf("[%s]\n",list->data);
// 		list = list->next;
// 	}

// }