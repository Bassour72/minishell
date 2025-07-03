#include "../../include/parsing.h"

int calculate_new_line_len(t_expand_token *tokens)
{
	int	len;

	len = 0;
	while (tokens)
	{
		if (tokens->data)
			len += ft_strlen(tokens->data) + 1;
		tokens = tokens->next;
	}
	return (len);
}

static int	is_valid(t_expand_token *token)
{
	if (token->join == 1 || (token->prev && token->prev->join == 1))
	{
		if (ft_strlen(token->data) == 2 && token->data[0] == token->data[1] && (token->data[0] == '\"' || token->data[0] == '\''))
			return (0);
	}
	return (1);
}

int expand_tokens_to_line(char **new_line, t_expand_token *tokens)
{
	int len;

	len = calculate_new_line_len(tokens) + 1;
	*new_line = ft_calloc(len, 1);
	if (!*new_line)
		return (perror("error: "), R_FAIL);
	while (tokens)
	{
		if(tokens->data)
		{
			if (is_valid(tokens))
			{
				ft_strlcat(*new_line, tokens->data, len);
				if(tokens->next && !tokens->join)
					ft_strlcat(*new_line, " ", len);
			}
		}
		tokens = tokens->next;
	}
	return (R_SUCCESS);
}