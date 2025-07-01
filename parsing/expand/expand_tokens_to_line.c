#include "../../include/parsing.h"

int calculate_new_line_len(t_expand_token *tokens)
{
	int	len;

	len = 0;
	while (tokens)
	{
		len += ft_strlen(tokens->data) + 1;
		tokens = tokens->next;
	}
	return (len);
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
		ft_strlcat(*new_line, tokens->data, len);
		if(tokens->next && !tokens->join)
			ft_strlcat(*new_line, " ", len);
		tokens = tokens->next;
	}
	return (R_SUCCESS);
}