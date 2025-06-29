#include "../../../include/parsing.h"


int is_valid_key_char(char c, int i)
{
	if (i == 0)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			return (1);
		else
			return (0);
	}
	else
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))
			return (1);
		else
			return (0);
	}
}

int extarct_var_key(char *str, char **key)
{
	int len;

	len = 0;
	if (str[0] == '?')
	{
		*key = ft_strdup("exit_status@gmail.com");
		if (!*key)
			return (perror("error: "), R_FAIL);
		return (R_SUCCESS);
	}//.......................................
	while (str[len] && is_valid_key_char(str[len], len))
		len++;
	if (len == 0)
		return (*key = NULL, R_SUCCESS); // i want the key to be no empty
	*key = ft_substr(str, 0, len);
	if (!*key)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

int extract_var_value(char *src, int *i, char **dst, t_env *env)
{
	char *key;
	char *value;

	if (extarct_var_key(src + 1, &key) == R_FAIL)
		return (R_FAIL);
	if (!key)
		*dst = NULL;
	value = env_get_value(env, key); //maybe this neef a free key
	if (ft_strcmp("exit_status@gmail.com", key) == 0)
		(*i) += 2;
	else
		(*i) += ft_strlen(key) + 1;
	free(key);
	if (value)
	{
		*dst = ft_strdup(value);
		if (!*dst)
			return (perror("error: "), R_FAIL);
	}
	else
	{
		*dst = NULL;
	}
	return (R_SUCCESS);
}

int is_valid_key(char *key)
{
	int	i;

	if (!key[0])
		return (0);
	i = -1;
	while (key[++i] && key[i] != '=')
	{
		if (is_valid_key_char(key[i], i) == 0)
		{
			// printf("not valid\n");
			return (0);
		}
	}
	return (1);
}

int is_befor_var_valid(t_expand_token *tokens)
{
	while (tokens->next)
		tokens = tokens->next;

	if (ft_strcmp(tokens->data, "=") == 0)
		tokens = tokens->prev;

	if (tokens->join == 1 && tokens->split == 1 && is_valid_key(tokens->data))
			return (1);

	// printf("not valid\n");	
	return (0);
}

int is_export(char *line)
{
	if (ft_memcmp("export ", line, 7) == 0)
		return (1);
	return (0);
}


int normal_variable_expander(char *str, int *i, t_expand_token **tokens, t_env *env)
{
	// printf("\033[34m-----------NORMAL VARIABLE--------------------------\033[0m\n");
	char	*data;
	int		join;
	int		split;

	data = NULL;
	if (!is_valid_key_char(str[*i + 1], 0) && str[*i + 1] != '?')
		return ((*i) += 2, R_CONTINUE);
	if (extract_var_value(str + *i, i, &data, env) == R_FAIL)
		return (R_FAIL);
	// printf("%d\n", *i);
	split = 1;
	if (is_export(str))
	{
		if (is_befor_var_valid(*tokens))
			split = 0;
	}
	join = 1;
	if (!str[*i] || str[*i] == ' ')
		join = 0;
	if (append_expand_token(tokens, data, 0, split, join) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}