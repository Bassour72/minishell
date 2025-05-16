#include "../../include/parsing.h"

void get_sub_str_len(char *str, int *i, int *quote, int *sub_len)
{
	while (str[*i] && str[*i] != '$')
	{
		update_quote(str[*i], quote);
		while (str[*i] && *quote == 1)
		{
			(*i)++;
			(*sub_len)++;
			update_quote(str[*i], quote);
		}
		(*i)++;
		(*sub_len)++;
	}
}

int extract_the_sub_str(char *str, int i, int *sub_len, char **new_str)
{
	char *tmp_str;
	char *normal_str;

	if (*sub_len != 0)
	{
		normal_str = ft_substr(str, i - *sub_len, *sub_len);
		if (!normal_str)
			return (perror("minishell: error: "), free(*new_str), 0);
		tmp_str = *new_str;
		*new_str = ft_strjoin(*new_str, normal_str);
		free(tmp_str);
		free(normal_str);
		if (!*new_str)
			return (perror("minishell: error: "), 0);
		*sub_len = 0;
	}
	return (1);
}

int expand_the_variable(char *str, int *i, char **new_str, t_env *env)
{
	char *key;
	char *value;
	char *tmp_str;

	key = extract_target_key(str + (*i) + 1);
	if (!key)
		return (perror("minishell: error: "), free(*new_str), 0);
	value = env_get_value(env, key);
	(*i) += ft_strlen(key) + 1;
	free(key);
	if (value)
	{
		//todo protect
		// tmp_str = value;
		value = ft_strjoin(value, "\"");
		// free(tmp_str);
		tmp_str = value;
		value = ft_strjoin("\"", value);
		free(tmp_str);
		tmp_str = *new_str;
		*new_str = ft_strjoin(*new_str, value);
		free(tmp_str);
		free(value);
		// printf("[%s]-> %p\n",tmp_str,  value);
		if (!*new_str)
			return (perror("minishell: error: "), 0);
	}
	return (1);
}

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
		if (!extract_the_sub_str(str, i, &sub_len, &new_str))
			return (free(str), NULL);
		if (quote != 1 && str[i] && str[i] == '$')
		{
			if (!expand_the_variable(str, &i, &new_str, env))
				return (free(str), NULL);
		}
		// printf("new_str%p\n", new_str);
	}
	// printf("\033[32mfin: [%s]\033[0m\n", new_str);
	free(str);
	return new_str;
}
