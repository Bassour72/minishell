#include "../../../include/parsing.h"

static int	calculate_data_len(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '$')
		i = 1;
	while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
		i++;
	return (i);
}

int normal_expander(char *str, int *i, t_expand_token **tokens)
{
	// printf("\033[34m-----------NORMAL--------------------------\033[0m\n");
	int		len;
	char	*data;
	int		join;

	len = calculate_data_len(str);
	data = ft_substr(str, 0, len);
	if (!data)
		return (perror("error: "), R_FAIL);
	join = 1;
	if (str[len] == ' ' || !str[len])
		join = 0;
	if (append_expand_token(tokens, data, 0, 1, join) == R_FAIL)
		return (R_FAIL);
	*i += len;
	return (R_SUCCESS);
}


// int extract_normal_word(char *str, int *i, char **data, int *quote)
// {
// 	int _i;

// 	_i = 0;
// 	while (1)
// 	{
// 		if (!str[_i] || str[_i] == '\"' || str[_i] == '\'' || str[_i] == '$' || str[_i] == ' ')
// 		{
// 			if (str[_i] == '\'')
// 				*quote = 1;
// 			else if (str[_i] == '\"')
// 				*quote = 2;
// 			*i += _i;
// 			break;
// 		}
// 		_i++;
// 	}
// 	*data = ft_substr(str, 0, _i);
// 	if (!*data)
// 		return (perror("error: "), printf("data is null at extract normal %d\n", _i), R_FAIL);
// 	return (R_SUCCESS);
// }

// int expand_normal(t_expand_funcs_data e)
// {
// 	int	join;

// 	join = 0;

// 	printf("\033[33m#Normal {%s}\033[0m\n", e.str );
// 	// if (e.data || 1)
// 	// 	exit(1);
// 	if (extract_normal_word(e.str + *e.i, e.i, e.data, e.quote) == R_FAIL)
// 		return (free_expand_tokens_list(*e.tokens), R_FAIL);
// 	printf("[%d]\n", *e.i);
// 	if (e.str[*e.i] && e.str[*e.i] != ' ' && e.str[*e.i])
// 		join = 1;
// 	if (append_expand_token(e.tokens, *e.data, e_WORD, *e.quote, join) == R_FAIL)
// 		return (free_expand_tokens_list(*e.tokens), R_FAIL);

// 	return (R_SUCCESS);
// }