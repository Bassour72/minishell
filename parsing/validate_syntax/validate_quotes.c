#include "../../include/parsing.h"

int validate_quotes(char *str)
{
	int i;
	char quote_type;

	quote_type = '\0';
	i = -1;
	while (str[++i])
	{
		if(quote_type == '\0' && (str[i] == '\'' || str[i] == '\"'))
			quote_type = str[i];
		else if (quote_type == str[i])
			quote_type = '\0';
	}
	if (quote_type != '\0')
	{
		if (quote_type == '\"')
			return (printf("syntax error near unexpected token `\"\n"), 0);
		else if (quote_type == '\'')
			return (printf("syntax error near unexpected token `\'\n"), 0);
	}
	return (1);
}
