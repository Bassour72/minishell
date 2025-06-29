#include "../../../include/parsing.h"

int double_quote_variable_expander(char *str, int *_i, char **data, t_env *env)
{
	// printf("\033[34m-----------DOUBLE QUOTE VARIABLE EXPANDER--------------------------\033[0m\n");
	int		join;
	int		split;
	char	*new_data;
	char	*tmp;
	
	new_data = NULL;
	if (!is_valid_key_char(str[*_i + 1], 0) && str[*_i + 1] != '?')
		return ((*_i) += 2, R_CONTINUE);
	if (extract_var_value(str + *_i, _i, &new_data, env) == R_FAIL)
		return (R_FAIL);
	tmp = *data;
	*data = ft_strjoin(*data, new_data);
	(free(tmp), free(new_data));
	if (!*data)
		return (R_FAIL);
	return (R_SUCCESS);
}