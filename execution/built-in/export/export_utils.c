#include "../../../include/execution.h"

  int ft_strcmp_v2(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	is_valid_identifier(const char *identifier, int *is_append_mode)
{
	int	i;
	int count_plus;

	if (!identifier)
		return (1);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (1);
	i = 1;
	count_plus = 0;
	while (identifier[i] != '\0' && identifier[i] != '=')
	{
		if (identifier[i] == '+')
		{
			if (identifier[i + 1] != '\0' && identifier[i + 1] == '=')
				count_plus++;
			else
				return (1);
		}
		else if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (1);
		if (count_plus > 1)
			return (1);
		i++;
	}
	*is_append_mode = count_plus;
	return (0);
}

