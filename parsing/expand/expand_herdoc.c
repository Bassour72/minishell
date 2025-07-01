#include "../../include/parsing.h"

int join_line(char **str1, char *str2)
{
	char *old_str1;

	old_str1 = *str1;
	*str1 = ft_strjoin(*str1, str2);
	free(old_str1);
	free(str2);
	if (!*str1)
		return (perror("error: "), R_FAIL);
	return (R_SUCCESS);
}

int expand_herdoc(char **herdoc_line, t_env *env)
{
	char *line;
	char *result = NULL;
	char *value = NULL;
	int		len;

	line = *herdoc_line;
	int i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0' && !ft_isspace(line[i + 1]) && line[i + 1] != '\'' && line[i + 1] != '\"')
		{
			
			if (is_valid_key_char(line[i + 1], 0))
			{
				if (extract_var_value(line + i + 1, &i, &value, env) == R_FAIL)
				{
					//cleaup and return;
					free(result);
					return (R_FAIL);
				}
				if (value)
				{
					if (join_line(&result, value) == R_FAIL)
					{
						//cleaup and return;
						return (R_FAIL);
					}
				}

			}
			else
				i +=2;
		}
		else
		{
			len = i + 1;
			while (line[len] && line[len] != '$')
				len++;
			value = ft_substr(line, i, len - i);
			if (!value)
			{
				//cleaup and return;
				free(result);
				return (R_FAIL);
			}
			if (join_line(&result, value) == R_FAIL)
			{
				//cleaup and return;
				return (R_FAIL);
			}
			i = len;
		}
	}
	free(*herdoc_line);
	*herdoc_line = result;
	return (R_SUCCESS);
}
