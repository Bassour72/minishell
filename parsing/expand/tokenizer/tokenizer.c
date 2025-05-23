#include "../../../include/expand.h"

static void check_quote(char c, int *quote)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
	{
		if (c == '\"')
			*quote = 2;
		else
			*quote = 1;
	}
	else if (*quote == 1 && c == '\'')
		*quote = 0;
	else if (*quote == 2 && c == '\"')
		*quote = 0;
}

int is_valid_key_char(char c, int i)
{
	if (i == 0)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')|| c == '_')
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
	while (str[len] && is_valid_key_char(str[len], len))
		len++;
	*key = ft_substr(str, 0, len);
	if (!*key)
		return (R_FAIL);
	// printf("key: [%s]\n", *key);
	return (R_SUCCESS);
}

int extract_var_value(char *src, int *i, char **dst, t_env *env)
{
	char *key;
	char *value;

	if (extarct_var_key(src, &key) == R_FAIL)
		return (R_FAIL);
	value = env_get_value(env, key);
	(*i) += ft_strlen(key) + 1;
	free(key);
	// printf("value: [%s]\n", value);
	if (value)
	{
		*dst = ft_strdup(value);
		if (!*dst)
			return (perror("error: "), R_FAIL);
	}
	else
	{
		*dst = malloc(1);
		if (!*dst)
			return (perror("error: "), R_FAIL);
		*dst[0] = '\0';
	}
	return (R_SUCCESS);
}

int extract_word_of_double_quote(char *str, int *i, char **data)
{
	int _i;
	int len;
	int start;

	len = 0;
	start = 0;
	_i = 0;
	if (str[0] == '\"')
	{
		start = 1;
		_i++;
	}
	while (str[_i])
	{
		if ((str[_i] == '$' && (is_valid_key_char(str[_i + 1], 0))) || str[_i] == '\"')
			break;
		len++;
		_i++;
	}
	(*i) += _i;
	if (len == 0)
		return (*data = NULL, R_SUCCESS);
	*data = ft_substr(str, start, len);
	if (!*data)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);

}


int extract_word_of_single_quote(char *str, int *i, char **data)
{
	int _i;
	int len;
	int start;

	len = 0;
	start = 0;
	_i = 0;
	if (str[0] == '\'')
	{
		start = 1;
		_i++;
	}
	while (str[_i])
	{
		if (str[_i] == '\'')
			break;
		len++;
		_i++;
	}
	(*i) += _i;
	*data = ft_substr(str, start, len);
	if (!*data)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

int extract_normal_word(char *str, int *i, char **data)
{
	int _i;
	int len;
	int start;

	len = 0;
	start = 0;
	_i = 0;
	while (str[_i])
	{
		if (!str[_i] || str[_i] == '\'' || str[_i] == '\"' || str[_i] == ' ' || (str[_i] == '$' && is_valid_key_char(str[_i + 1], 0)))
			break;
		len++;
		_i++;
	}
	(*i) += _i;
	*data = ft_substr(str, start, len);
	if (!*data)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

int tokenize(char *str, t_expand_token **tokens, t_env *env)
{
	int quote;
	int i;
	char *data = NULL;
	int join = 0;

	quote = 0;
	i = 0;
	while (str[i])
	{
		data = NULL;
		join = 0;
		check_quote(str[i], &quote);
		// printf("\033[33mquote: [%d] at [%d]\033[0m\n", quote, i);
		// printf("\033[33mstr[i]: [%c] at: [%d] and quote: [%d]\033[0m\n", str[i], i, quote);
		if (quote != 1 && str[i] == '$' && is_valid_key_char(str[i + 1], 0))
		{
			// printf("var\n");
			if (extract_var_value(str + i + 1, &i, &data, env) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
			if (str[i] != ' ' || quote != 0)
				join = 1;
			if (append_expand_token(tokens, data, VAR, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else if (quote == 2)
		{
			// printf("double\n");
			if (extract_word_of_double_quote(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
			if (data)
			{
				if (str[i + 1] != ' ')
					join = 1;
				if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
					return (free_expand_tokens_list(*tokens), R_FAIL);
			}
		}
		else if (quote == 1)
		{
			// printf("single\n");
			if (extract_word_of_single_quote(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
			if (str[i] != ' ')
				join = 1;
			if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else if (quote == 0 && str[i] != ' ' && str[i] != '\"' && str[i] != '\'')
		{
			// printf("normal\n");
			if (extract_normal_word(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
			if (str[i] != ' ' && str[i])
				join = 1;
			if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else
			i++;
		// printf("\033[32mfinal data = {%s}\033[0m\n", data);

	}
	return (R_SUCCESS);
}
