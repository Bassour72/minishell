#include "../../include/parsing.h"

void get_sub_str_len(char *str, int *i, int *quote, int *sub_len)
{
	update_quote(str[*i], quote);
	if ((*quote == 2 || *quote == 0) && str[*i] != '$')
		(*i)++;
	while (str[*i] && str[*i] != '$')
	{
		update_quote(str[*i], quote);
		while (str[*i] && *quote == 1)
		{
			(*i)++;
			(*sub_len)++;
			// update_quote(str[*i], quote);
		}
		(*i)++;
		(*sub_len)++;
	}
}

int extract_the_sub_str(char *str, int i, int sub_len, char **new_str)
{
	char *tmp_str;
	char *normal_str;

	if (sub_len != 0)
	{
		normal_str = ft_substr(str, i - sub_len, sub_len);
		if (!normal_str)
			return (perror("minishell: error: "), free(*new_str), 0);
		tmp_str = *new_str;
		*new_str = ft_strjoin(*new_str, normal_str);
		free(tmp_str);
		free(normal_str);
		if (!*new_str)
			return (perror("minishell: error: "), 0);
		sub_len = 0;
	}
	return (1);
}

int wrap(char **str)
{
	char *tmp;

	tmp = *str;
	*str = ft_strjoin("\"", *str);
	free(tmp);
	if (!*str)
		return (0);
	tmp = *str;
	*str = ft_strjoin(*str, "\"");
	free(tmp);
	if (!*str)
		return (0);
	return (1);
}

int expand_the_variable(char *str, int *i, char **new_str, t_env *env)
{
	char *key;
	char *value;
	char *tmp_str;

	// printf("expand var\n");
	key = extract_target_key(str + (*i) + 1);
	if (!key)
		return (perror("minishell: error: "), free(*new_str), 0);
	value = env_get_value(env, key);
	int dd = ft_strlen(key) + 1;
	printf("len :%d\n", dd);
	(*i) += dd;
	free(key);
	if (value)
	{
		// int h = -1;
		// while (value[++h])
		// {
		// 	if (value[h] == '\'')
		// 	{
		// 		// tmp_str = value;
		// 		value = ft_strjoin(value, "\"");
		// 		// printf("v:{%s}\n", *new_str);
		// 		// free(tmp_str);
		// 		tmp_str = value;
		// 		value = ft_strjoin("\"", value);
		// 		free(tmp_str);
		// 		break;
		// 	}
		// }

		//todo protect
		tmp_str = *new_str;
		*new_str = ft_strjoin(*new_str, value);
		free(tmp_str);
		// free(value);
		// printf("[%s]-> %p\n",*new_str,  value);
		if (!*new_str)
			return (perror("minishell: error: "), 0);
	}
	return (1);
}

int exp(char *str, int *i, t_node **list)
{
	char *key;
	char *value;

	key = extract_target_key(str);
	if (!key)
		return (0);
	value = env_get_value(*list, key);
	(*i) += ft_strlen(key);
	free(key);
	if (!value)
		return (0);
	char *data = ft_strdup(value);
	if (!data)
		return (0);
	append_node(list, &data);
	return (0);
}


int norm(char *str, int *i, t_node **list, int *quote)
{
	int j;

	j = 0;
	if	(*quote == 1)
	{
		while (str[++j] != quote)
			;
	}
	else if (*quote == 2)
	{
		while (str[++j] != quote  && str[j] == '$' && (str[j + 1] != ' ' && str[j + 1] != '\"' && str[j + 1] != '\''))
			;
	}

}
char *do_expantion(t_env *env, char *str)
{
	int 	i;
	char	*new_str;
	t_node *list;
	int		sub_len;
	int		quote;

	quote = 0;
	i = 0;
	while (str[i])
	{
		update_quote(str[i],&quote);
		if (quote != 1 && str[i] == '$' && str[i + 1] != ' ' && str[i + 1] != '\0')
		{
			i++;
			exp(str, &i, &list);
		}
		else
		{
			norm(str, &i, &list, &quote);
		}
		
	}
	// printf("\033[32mfin: [%s]\033[0m\n", new_str);
	free(str);
	return new_str;
}


/*
char *do_expantion(t_env *env, char *str)
{
	int 	i;
	char	*new_str;
	int		sub_len;
	int		quote;

	quote = 0;
	new_str = NULL;
	sub_len = 0;
	i = 0;
	while (str[i])
	{
		get_sub_str_len(str, &i, &quote, &sub_len);
		// printf("strlen :%d\n", sub_len);
		if (!extract_the_sub_str(str, i, sub_len, &new_str))
			return (free(str), NULL);
		if (quote != 1 && str[i] == '$')
		{
			if (str[i + 1] == ' ' || !str[i + 1])
			{
				if (!extract_the_sub_str(str, ++i, 1, &new_str))
					return (free(str), NULL);
			}
			else if (!expand_the_variable(str, &i, &new_str, env))
				return (free(str), NULL);
		}
		// printf("new_str%p\n", new_str);
	}
	// printf("\033[32mfin: [%s]\033[0m\n", new_str);
	free(str);
	return new_str;
}*/
