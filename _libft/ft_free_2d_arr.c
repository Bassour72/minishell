#include "libft.h"

void	free_2d_arr(char **arr)
{
	char	**original_arr;
	char	*tmp;

	original_arr = arr;
	while (*arr)
	{
		tmp = *arr;
		arr++;
		free(tmp);
	}
	free(original_arr);
}
