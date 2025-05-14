#include "../../include/parsing.h"

static int create_new_data(char *str, char **data, int *i)
{
	int _i;
	int len;
	char *_data;
	char qoute_type;

	_i = (*i) + 1;
	qoute_type = *(str + *i);
	len = 2;
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
	*data = _data;
	(*i) = _i + len;
	return (1);
}

static int join_data_to_old_data(char *str, char **data, int *i)
{
	int len;
	int _i;
	int j;
	char *_data;
	char qoute_type;

	_i = (*i) + 1;
	qoute_type = *(str + *i);
	len  = ft_strlen(*data);
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
	ft_strlcpy(_data, *data, len + 1);
	ft_strlcpy(_data + len, str + _i, j + 1);
	free(*data);
	*data = _data;
	(*i) = _i + j;
	return (1);
}

int	m_quotes(char *str, char **data, int *i)
{
	char	*_data;
	char	qoute_type;
	int		_i;
	int		len;
	int		j;

	j = 0;
	len = 0;
	_i = *i;
	qoute_type = *(str + _i);
	if (!*data)
	{
		create_new_data(str, data, i);
	}
	else
	{
		join_data_to_old_data(str, data, i);
	}
	return (1);
}
