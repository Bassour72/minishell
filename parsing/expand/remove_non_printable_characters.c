#include "../../include/expand.h"
#include <string.h>

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

int allocate_double_arr_size(char ***new_data, char **old_data)
{
	int size;

	size = -1;
	while (*(old_data + ++size))
		;
	*new_data = malloc(sizeof(char *) * (size + 1));
	if (!new_data)
		return (perror("error: "), R_FAIL);
	while (size >= 0)
		*(*new_data + size--) = NULL;
	return (R_SUCCESS);
}

int generate_new_data_str(char **dst, char *str)
{
	int size;
	int i;
	
	i = -1;
	size = 0;
	while (*(str + ++i))
	{
		if (*(str + i) != DOUBLE_QUOTE && *(str + i) != SINGLE_QUOTE)
			size++;
	}
	// printf("size: %d\n",size);
	*dst = malloc(size + 1);
	if (!*dst)
		return (perror("error: "), R_FAIL);
	i = 0;
	while (*str)
	{
		if (*str != DOUBLE_QUOTE && *str != SINGLE_QUOTE)
		{
			*(*dst + i++) = *str;
			// printf("[%c]->\n", *(*dst + i - 1));
		}
		str++;
	}
	*(*dst + i) = '\0';
	return (R_SUCCESS);
}

int	remove_non_printable_characters(char ***old_data)
{
	char	**new_data;
	int		i;

	if (allocate_double_arr_size(&new_data, *old_data) == R_FAIL)
		return (free_2d_arr(*old_data), *old_data = NULL, R_FAIL);
	i = -1;
	while (*(*old_data + ++i))
	{
		if (generate_new_data_str(new_data + i, *(*old_data + i)) == R_FAIL)
		{
			(free_2d_arr(new_data), free_2d_arr(*old_data));
			return (R_FAIL);
		}
	}
	free_2d_arr(*old_data);
	*old_data = new_data;
	return (R_SUCCESS);
}

//---------TESTS-----------------

void print_array(char **arr)
{
	while (*arr)
		printf("'%s'\n", *arr++);
}

void test_remove_non_printable_characters()
{
	char **input = malloc(sizeof(char *) * 4);
	input[0] = strdup("hel" "\x0E" "lo");			// "hello"
	input[1] = strdup("wor" "\x0F" "ld");			// "world"
	input[2] = strdup("noquote");					// "noquote"
	input[3] = NULL;

	if (remove_non_printable_characters(&input) == R_FAIL)
	{
		printf("Test failed: function returned R_FAIL\n");
		return;
	}

	if (
		strcmp(input[0], "hello") == 0 &&
		strcmp(input[1], "world") == 0 &&
		strcmp(input[2], "noquote") == 0 &&
		input[3] == NULL
	)
		printf("Test passed: remove_non_printable_characters\n");
	else
	{
		printf("Test failed:\n");
		print_array(input);
	}

	free_2d_arr(input);
}

// int main()
// {
// 	test_remove_non_printable_characters();
// 	return 0;
// }