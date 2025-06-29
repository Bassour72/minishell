#include "../../../include/parsing.h"

static int	calculate_data_len(char *str)
{
	int	i;

	i = 1;
	while (str[i] != '\'')
		i++;
	return (i + 1);
}

int	single_quote_expander(char *str, int *i, t_expand_token **tokens)
{
	char	*data;
	int		len;
	int		_i;
	int		join;

	len = calculate_data_len(str);
	data = ft_substr(str, 0, len);
	if (!data)
		return (perror("error: "), R_FAIL);
	data[0] = SINGLE_QUOTE;
	data[len - 1] = SINGLE_QUOTE;
	join = 1;
	if (str[len] && str[len] == ' ')
		join = 0;

	if (append_expand_token(tokens, data, 1, 0, join) == R_FAIL)
		return (R_FAIL);
	*i += len;
	return (R_SUCCESS);	
}
