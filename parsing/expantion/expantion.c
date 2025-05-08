#include "../../include/parsing.h"

char *extract_target_key(char *str)
{
	char *key;
	int len;

	if (*str == '$')
		len = 1;
	else
	{
		len = 0;
		while (str[len] && str[len] != ' ' && str[len] != '\"' && str[len] != '\'' && str[len] != '$' && !is_special(str + len))
			len++;
	}
	key = ft_substr(str, 0, len);
	return (key);
}

void expand_double_quotes(t_env *env, char **data, char *str, int *i)
{
	int _i;
	char *tmp;
	char *_data;
	char *key;

	key = NULL;
	_i = 1;
	while (str[_i] && str[_i] != '\"') //note if the double qoutes are not closed undefined behaviour will appear
	{
		key = extract_target_key(str + _i);
		printf("\033[32mkey: %s\033[0m\n", key);
		if (!*data)
		{
			*data = env_get_value(env, key);
			printf("#%s\n",*data);
		}
		else
		{
			tmp = env_get_value(env, key);
			_data = malloc(ft_strlen(*data) * ft_strlen(tmp) + 1);
			ft_strlcat(_data, *data, ft_strlen(*data));
			ft_strlcat(_data, tmp, ft_strlen(tmp));
			free(*data);
			free(tmp);
			*data = _data;
		}
		_i++;
	}
	printf("i: %d\n", _i);
	*i = _i ;
}

int get_str_len(char *str, char *set)
{
	int i;
	int j;

	i = 0;
		printf(">>>[%c]\n", *(str + i));
	while (*(str + i) != '\0')
	{
		j = -1;
		while(*(set + ++j) != '\0')
		{
			if (*(set + j) == *(str + i))
				return (i);
		}
		i++;
	}
	return (i);
}

int handle_normal(char *str, int *i, char **data)
{
	char *start;
	char *_data;
	int len;

	start = str;
	len = get_str_len(str, "\'\"$");
	if (!*data)
	{
		_data = malloc(len + 1);
		if (!_data)
			return (0);
		ft_strlcpy(_data, str, len + 1);
	}
	else
	{
		_data = malloc(ft_strlen(*data) + len + 1);
		if (!_data)
			return (0);
		ft_strlcpy(_data, *data, ft_strlen(*data) + 1);
		ft_strlcpy(_data + ft_strlen(*data), str, len + 1);
		free(*data);
	}
	*i += len; 
	*data = _data;
	return (1); 
}

int handle_double_quotes(char * str, int *i, char **data)
{
	int len;
	char *_data;

	len = get_str_len(str, "$\"");

	if (!*data)
	{
		_data = malloc(len + 1);
		if (!_data)
			return (0);
		ft_strlcpy(_data, str, len + 1);
	}
	else
	{
		_data = malloc(ft_strlen(*data) + len + 1);
		if (!_data)
			return (0);
		ft_strlcpy(_data, *data, ft_strlen(*data) + 1);
		ft_strlcpy(_data + ft_strlen(*data), str, len + 1);
		free(*data);
	}

	*data = _data;
	*i += len + 2;
	return (1);
}

int handle_single_quote(char *str, int *i, char **data)
{
	int		len;
	char	*_data;

	len = get_str_len(str, "\'");
	if (!*data)
	{
		_data = malloc(len + 1);
		if(!_data)
			return (0);
		ft_strlcpy(_data, str, len + 1);
	}
	else
	{
		_data = malloc (ft_strlen(*data) + len + 1);
		if (!_data)
			return (0); //note need to free *data
		ft_strlcpy(_data, *data, ft_strlen(*data) + 1);
		ft_strlcpy(_data + ft_strlen(*data), str, len + 1);
	}
	*data = _data;
	*i += len + 2;
	return (1);
}

void expand_one_str(t_env *env, t_node *list, char *str)
{
	int i;
	int start;
	char *data;

	data = NULL;
	i = 0;
	start = 0;

	while (str[i])//note expand each one independent[]
	{
		printf("current [%c]\n", str[i]);
		if (str[i] == '\"')
			handle_double_quotes(str + i + 1, &i, &data);
		else if (str[i] == '\'')
			handle_single_quote(str + i, &i, &data);
		// else if (str[i] == '$')
		// 	handle_dolar_sign(str + i, i, data);
		else
			handle_normal(str + i, &i, &data);

		// i++;÷
	}
	printf("final data: %s\n", data);
}


void expand_data_variables(t_env *env, char **old_data)
{
	t_node	*list;
	int		i;

	list = NULL;
	i = -1;

	while (old_data[++i])//note loop throw all data on the node data[][]
	{
		expand_one_str(env, list, old_data[i]);

	}
}

void expand_variables(t_tree *root, t_env *env) //note this is to expand data[][] 
{
	if (root->data)
		expand_data_variables(env, root->data);
	if (root->right)
		expand_variables(root->right, env);
	if (root->left)
		expand_variables(root->left, env);
}