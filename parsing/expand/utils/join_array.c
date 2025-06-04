#include "../../../include/parsing.h"


static int calculate_len(char **arr)
{
	int i;
	int j;
	int	len;

	i = -1;
	while (arr[++i])
	{
		j = -1;
		while (arr[i][++j])
			len++;
		len++;
	}
	return (len);
}

static void cpy_to_songle_arr(char **double_arr, char **single_arr, int len)
{
	int i;

	i = -1;
	while (double_arr[++i])
	{
		ft_strlcat(*single_arr, " ", len);
		ft_strlcat(*single_arr, double_arr[i], len);
	}

}


int join_arr(char **arr, char **joined_arr)
{
	int len;
	int i;

	len = calculate_len(arr);
	*joined_arr = malloc(len + 1);
	if (!*joined_arr)
		return (perror("error: "), R_FAIL);

	ft_bzero(*joined_arr, len + 1);
	cpy_to_songle_arr(arr, joined_arr, len + 1);
	return (R_SUCCESS);
}