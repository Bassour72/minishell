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
		else if (!ft_isalnum(identifier[i]) && identifier[i] != '_') //todo
			return (1);
		if (count_plus > 1)
			return (1);
		i++;
	}
	*is_append_mode = count_plus;
	return (0);
}
 char *get_env_key(const char *identifier)
{
	char *new_key;
	int len = 0;
	int i;
	if (!identifier)
		return NULL;
	while (identifier[len] && identifier[len] != '=' && identifier[len] != '+')
		len++;

	new_key = malloc(len + 1);
	if (!new_key)
		return NULL;
	i = 0;
	while (i < len)
	{
		new_key[i] = identifier[i];
		i++;
	}
	new_key[i] = '\0';

	return new_key;
}

 char *get_env_value1(const char *identifier)
{
	int i = 0;

	if (!identifier)
		return (NULL);

	while (identifier[i] && identifier[i] != '=')
		i++;

	if (!identifier[i])
		return (NULL);

	return (ft_strdup(identifier + i + 1));
}

 void swap_node(t_env *a, t_env *b)
{
	if (!a || !b)
		return;
	char *temp_key = a->key;
	char *temp_value = a->value;
	bool temp_exported = a->exported;
	a->key = b->key;
	a->value = b->value;
	a->exported = b->exported;
	b->key = temp_key;
	b->value = temp_value;
	b->exported = temp_exported;
}