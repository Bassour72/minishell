#include "../../../include/parsing.h"

int tokenize(char *str, t_expand_token **tokens, t_env *env)
{
	int i;
	int state;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (single_quote_expander(str + i, &i, tokens) ==R_FAIL)
				return (R_FAIL);
		}
		else if (str[i] == '\"')
		{
			if (double_quote_expander(str + i, &i, tokens, env) == R_FAIL)
				return (R_FAIL);
		}
		else if (str[i] == '$' && str[i + 1] && str[i + 1] != '\"' && str[i + 1] != '\'' && str[i + 1] != ' ')
		{
			state = normal_variable_expander(str, &i, tokens, env);
			if (state == R_CONTINUE)
				continue;
			else if (state == R_FAIL)
				return (R_FAIL);
		}
		else if (str[i] == ' ')
			i++;
		else
		{
			if (normal_expander(str + i, &i, tokens) == R_FAIL)
				return (R_FAIL);
		}
	}
	return (R_SUCCESS);
}
