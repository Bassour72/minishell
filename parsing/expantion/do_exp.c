#include "../../include/parsing.h"

static int append_single(char *str, char **new_data, int *_i)
{
	int		len;
	char	*s2;
	char	*tmp;
	int		i;

	len = 0;
	i = 0;
	while (str[++i] != '\'')
		len++;
	(*_i) += i; //maybe +1 '
	if (*new_data == NULL)
	{
		new_data = ft_substr(str, 1, len);
		if (!new_data)
			return (0);
		printf("append_single :[%s] | sizeof [%d] | i:[%d]\n", new_data, len, i);
		return (1);
	}
	s2 = ft_substr(str, 1, len);
	if (!s2)
		return (0);
	printf("append_single :[%s] | sizeof [%d] | i:[%d]\n", new_data, len, i);
	tmp = *new_data;
	*new_data = ft_strjoin(new_data, s2);
	free(s2);
	free(tmp);
	if (!new_data)
		return (0);
}

static int calulate_len(char *str, t_env *env, int *len)
{
	int		i;
	char	*key;
	char	*value;

	i = -1;
	while (str[++i] && str[i] != '\"')
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			key = extract_target_key(str);
			if (!key)
				return (0);
			value = env_get_value(env, key);
			i += ft_strlen (key);
			free(key);
			if (value)
				(*len) += ft_strlen(value);
		}
		(*len)++;
	}
	return (1);
}

static int append_double(char *str, char **new_data, int *_i, t_env *env)
{
	int len;
	int i;

	len = 0;
	if (!calulate_len(str + 1, env, &len))
		return (0);
	printf("double len: %d\n", len);

	gen_new_data
	{
		
	}
	


}

static int exp_node(t_node *node, t_env *env)
{
	char	*str;
	char	*new_data;
	int		quote;
	int 	i;

	new_data = NULL;
	str = node->data;
	i = -1;
	while (str[++i])
	{
		update_quote(str[i], &quote);
		if (quote == 1)
		{
			append_single(str + i, &new_data, &i);
		}
		else if (quote == 2)
		{
			append_double(str + i, &new_data, &i, env);
		}
	}
}

int do_exp(t_node **list, t_env *env)
{
	t_node *node;

	node = *list;
	while (node)
	{
		if (!exp_node(node, env))
			return (0);//review
		node = node->next;
	}
}