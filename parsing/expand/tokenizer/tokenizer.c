#include "../../../include/parsing.h"

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
	// if (str[0] == '$')
	// 	return (ft_strdup("$"));
	while (str[len] && is_valid_key_char(str[len], len))
		len++;
	// if (len == 0)
	// 	return (*key = NULL, R_SUCCESS);
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
		*dst = NULL;
		// if (!*dst)
		// 	return (perror("error: "), R_FAIL);
		// *dst[0] = '\0';
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
		if ((str[_i] == '$' && (is_valid_key_char(str[_i + 1], 0) || (str[_i + 1] >= '0' && str[_i + 1] <= '9') || str[_i + 1] == '$')) || str[_i] == '\"' )
			break;
		len++;
		_i++;
	}
	(*i) += _i;
	
	*data = ft_substr(str, start, len);
	// printf("data [%s]\n", *data);
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
	// printf("STR<%s>\n", str);
	while (str[_i])
	{
		if (str[_i] != '\'' && str[_i] != '\"' && str[_i] != ' ')
		{
			if (str[_i] == '$' && str[_i + 1] != ' ' && str[_i + 1] != '\0' )
				break;
			_i++;
			len++;
		}
		else
			break;
		// if (!str[_i] || str[_i] == '\'' || str[_i] == '\"' || str[_i] == ' ' || (str[_i] == '$' && str[_i + 1] != ' '))
		// 	break;
		// len++;
		// _i++;
	}
	// printf("LEN<%d>\n", len);
	if (len == 0)
		_i += 1;
	(*i) += _i;
	*data = ft_substr(str, start, len);
	if (!*data)
		return (perror("error: "), R_FAIL);
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
	t_expand_token *target; //must be null
	t_expand_token *equal; //must be =
	t_expand_token *key; 

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

int tokenize(char *str, t_expand_token **tokens, t_env *env)
{
	
	int quote;
	int i;
	char *data = NULL;
	int join = 0;
	int split;

	quote = 0;
	i = 0;
	while (str[i])
	{
		data = NULL;
		join = 0;
		check_quote(str[i], &quote);
		// printf("\033[33mquote: [%d] at [%d]\033[0m\n", quote, i);
		// printf("\033[33mstr[i]: [%c] at: [%d] and quote: [%d]\033[0m\n", str[i], i, quote);
		if (quote != 1 && str[i] == '$' && str[i + 1] && str[i + 1] != '\"' && str[i + 1] != '\'' && str[i + 1] != ' ')
		{
				// printf("\033[33m#var {%s}\033[0m\n", str + i);
			if (!is_valid_key_char(str[i + 1], 0))
			{
				if (quote == 2)
				{
					if (extract_word_of_double_quote(str + i, &i, &data) == R_FAIL)
						return (free_expand_tokens_list(*tokens), R_FAIL);
					if (str[i + 1] != ' ')
						join = 1;
					if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
						return (free_expand_tokens_list(*tokens), R_FAIL);
				}
				if (str[i + 1] == '\'' || str[i + 1] =='\"')
					i++;
				else
					i += 2;
				continue;
			}
			split = quote;
			// printf("[%s]<<<<<<<<\n", str);
			if (is_export(str))
			{
				if (extract_var_value(str + i + 1, &i, &data, env) == R_FAIL)
					return (free_expand_tokens_list(*tokens), R_FAIL);
				if (str[i] != ' ' && str[i] != '\"')
					join = 1;
				if (is_befor_var_valid(*tokens))
					split = 2;
					// printf("split: %d| {%s}\n", split , data);

			}
			else
			{
				if (extract_var_value(str + i + 1, &i, &data, env) == R_FAIL)
					return (free_expand_tokens_list(*tokens), R_FAIL);
				if (str[i + 1] != ' ' && str[i + 1] != '\"')
					join = 1;
			}
			if (append_expand_token(tokens, data, VAR, split, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else if (quote == 2)
		{
			// printf("double\n");
			if (extract_word_of_double_quote(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);

			if (str[i + 1] != ' ')
				join = 1;
			if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);

		}
		else if (quote == 1)
		{
			// printf("single\n");
			if (extract_word_of_single_quote(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
			if (str[i + 1] != ' ')
				join = 1;
			if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else if (quote == 0 && str[i] != ' ' && str[i] != '\"' && str[i] != '\'')//todo check str[i] != ' ' && str[i] != '\"' && str[i] != '\''
		{
			// printf("\033[33m#Normal {%s}\033[0m\n", str + i);
			if (extract_normal_word(str + i, &i, &data) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
				// printf("[%c]\n", str[i]);
			if (str[i] != ' ' && str[i])
				join = 1;
			if (append_expand_token(tokens, data, e_WORD, quote, join) == R_FAIL)
				return (free_expand_tokens_list(*tokens), R_FAIL);
		}
		else
			i++;
		// printf("\033[32mfinal data = {%s}\033[0m\n", data);
		// join = 0;
	}
	return (R_SUCCESS);
}
