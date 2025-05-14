
#include "../../include/parsing.h"


int get_normal_len(char *str, int *_i)
{
	int len;

	len  = 0;
	while (*(str + *_i) && *(str + *_i) != ' ' && *(str + *_i) != '\'' && *(str + *_i) != '\"' && !is_special(str + *_i))
	{
		len++;
		(*_i)++;
	}
	return (len);
}

static int create_new_data(char *str, char **data, int *i)
{
	int _i;
	int len;
	char *_data;

	_i = *i;
	len  = 0;
	len = get_normal_len(str, &_i);
	_i -= len;
	_data = malloc(len + 1);
	if (!_data)
		return (0);
	ft_strlcpy(_data, str + _i, len + 1);
	*data = _data;
	(*i) = _i + len;
	return (1);
}

static int join_data_to_old_data(char *str, char **data, int *i)
{
	int		j;
	int		len;
	int		_i;
	char	*_data;

	_i = *i;
	j = ft_strlen(*data);
	len = get_normal_len(str, &_i);
	_i -= len;
	_data = malloc(j + len + 1);
	if (!_data)
		return (free(*data), 0);
	ft_strlcpy(_data, *data, j + 1);
	ft_strlcpy(_data + j, str + _i  , len + 1);
	free(*data);
	*data = _data;
	(*i) = _i + len;
	return (1);
}

int m_normal(char *str, char **data, int *i)
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
		create_new_data(str, data, i);
	}
	else
	{
		join_data_to_old_data(str, data, i);	
	}
	return (state);
}
