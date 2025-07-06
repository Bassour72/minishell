#include "../../../include/execution.h"

 void	update_existing_env(t_env *existing, char *new_value, int append_mode)
{
	char *temp_value;

	if (new_value)
	{
		if (append_mode)
		{
			temp_value = ft_strjoin(existing->value, new_value);
			free(existing->value);
			free(new_value);
			existing->value = temp_value;
			return ;
		}
		free(existing->value);
		existing->value = new_value;
	}
}