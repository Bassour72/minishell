#include "../../include/parsing.h"

static int check_quote(int *quote, char c)
{
	if (c == '\'' || c == '\"')
	{
		if (*quote == 0)
			*quote = c;
		else if(*quote == c)
			*quote = 0;
	}
	return (*quote);
}

static int calculate_delimiter_len_and_replace_dolars(char *str)
{
	int	len;
	int i;
	int quote;

	quote = 0;
	len = 0;
	i = -1;
	while (*(str + ++i))
	{
		if (*(str + i) == '$' && check_quote(&quote, *(str + i + 1)) == 0 && \
		*(str + i + 1) && !ft_isspace(*(str + i + 1)) && (*(str + i + 1) == '\"' || \
		*(str + i + 1) == '\''))
			*(str + i) = DOUBLE_QUOTE;
		else
			len++;
	}
	return (len);
}

char	*delimiter_clear_dollar(char *str)
{
	int		len;
	char	*new_str;
	int		i;
	int		j;

	len = calculate_delimiter_len_and_replace_dolars(str);
	new_str = malloc(len + 1);
	if (!new_str)
		return (perror("error: "), NULL);
	
	i = -1;
	j = 0;
	while (*(str + ++i))
	{
		if (*(str + i) != DOUBLE_QUOTE)
		{
			*(new_str + j) = *(str + i);
			j++;
		}
	}
	*(new_str + j) = '\0';
	free(str);
	return (new_str);
}
