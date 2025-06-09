
#include "../../include/parsing.h"


int get_normal_len(char *str)
{
	int len;

	len  = 0;
	while (*(str + len) && *(str + len) != ' ' && *(str + len) != '\'' && *(str + len) != '\"' && !is_special(str + len))
	{
		len++;
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
	len = get_normal_len(str + *i);
	(*i) = _i + len;
	_data = malloc(len + 1);
	if (!_data)
		return (perror("error: "), R_FAIL);
	ft_strlcpy(_data, str + _i, len + 1);
	*data = _data;
	
	return (R_SUCCESS);
}

static int join_data_to_old_data(char *str, char **data, int *i)
{
	int		j;
	int		len;
	int		_i;
	char	*_data;

	_i = *i;
	j = ft_strlen(*data);
	len = get_normal_len(str);
	(*i) = _i + len;
	_data =NULL;// malloc(j + len + 1);
	if (!_data)
		return (perror("error: "), free(*data), R_FAIL);
	ft_strlcpy(_data, *data, j + 1);
	ft_strlcpy(_data + j, str + _i  , len + 1);
	free(*data);
	*data = _data;
	
	return (R_SUCCESS);
}

int m_normal(char *str, char **data, int *i)
{
	char	*_data;
	int		_i;
	int		len;
	int		j;

	j = 0;
	len = 0;
	_i = *i;

	if (!*data)
	{
		if (create_new_data(str, data, i) == R_FAIL)
			return (R_FAIL);
	}
	else
	{
		if (join_data_to_old_data(str, data, i) == R_FAIL)
			return (R_FAIL);	
	}
	return (R_SUCCESS);
}
