#include "../../../include/parsing.h"

int calculate_new_data_len(char *str, int _i)
{
	int	len;

	len = 0;
	if (_i == 0)
		len++;
	while (1)
	{
		// printf("%c\n", str[len]);
		if (str[len] == '\"')
		{
			len++;
			break;
		}
		else if (str[len] == '$' && str[len + 1] != ' ' && str[len + 1] != '\"')
		{
			break;
		}
		len++;
	}
	return (len);
}

int extract_data(char *str, int *_i, char **data)
{
	// printf("\033[34m-----------DOUBLE QUOTE--------------------------\033[0m\n");
	char	*new_data;
	char	*tmp;
	int		len;

	len = calculate_new_data_len(str, *_i);
	// if (*data == NULL)
	// 	len++;
		// printf("--%d\n",len);
	(*_i) += len; 
	new_data = ft_substr(str, 0, len);
	if (!new_data)
		return (R_FAIL);
	tmp = *data;
	*data = ft_strjoin(*data, new_data);
	// print(*data);
	(free(tmp), free(new_data));
	if (!*data)
		return (R_FAIL);
	len  = ft_strlen(*data);
	if (len > 0 && (*data)[len - 1] == '\"')
		return (R_DONE);
	return (R_SUCCESS);
}

int is_done(char *data)
{
	int	len;

	len = ft_strlen(data);
	if (len >= 2 && data[len - 1] == '\"')
		return (1);
	return (0);
}

int	double_quote_expander(char *str, int *i, t_expand_token **tokens, t_env *env)
{
	
	int		_i;
	int		state;
	char	*data;
	int		join;

	data = NULL;
	_i = 0;
	while (1)
	{
		if (str[_i] == '$' && str[_i + 1] != '\"' && str[_i + 1] != ' ' )
		{
			if (double_quote_variable_expander(str, &_i,  &data, env) == R_FAIL)
				return (R_FAIL);
		}
		else
		{
			if (extract_data(str + _i, &_i, &data) == R_FAIL)
				return (R_FAIL);
			else if (is_done(data))
				break;
		}
	}
	(*i) += _i;
	join = 1;
	if (str[_i] == ' ' || !str[_i])
		join = 0;
	data[0] = DOUBLE_QUOTE;
	data[ft_strlen(data) - 1] = DOUBLE_QUOTE;
	if (append_expand_token(tokens, data,2, 0, join) == R_FAIL)
		return (R_FAIL);
	return (R_SUCCESS);
}




















































































// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>

// #define DOUBLE_QUOTE 15
// #define R_SUCCESS 0
// #define R_FAIL 1

// char *ft_substr(char *src, int start, int len)
// {
// 	char *out = malloc(len + 1);
// 	if (!out) return NULL;
// 	memcpy(out, src + start, len);
// 	out[len] = '\0';
// 	return out;
// }

// int is_valid_key_char(char c, int start)
// {
// 	(void)start;
// 	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
// }

// char *ft_substr(char *src, int start, int len)
// {
// 	char *out = malloc(len + 1);
// 	if (!out) return NULL;
// 	memcpy(out, src + start, len + 1);
// 	out[len] = '\0';
// 	return out;
// }

// static int extract_word_of_double_quote(char *str, int *i, char **data, int *quote)
// {
// 	int	_i;

// 	if (str[0] == '\"')
// 		str[0] = DOUBLE_QUOTE;
// 	_i = 0;
// 	while (1)
// 	{
// 		printf("inner i:%d\n", _i);
// 		if(str[_i] == DOUBLE_QUOTE && *quote == 2 && *i != 0) // != 0 to skipp the first char in the input
// 		{
// 			*quote = 0;
// 			if (str[_i + 1] == '\'')
// 				*quote = 1;
// 			_i++;
// 			*i += _i;
// 			break;
// 		}
// 		if (str[_i] == '\"')
// 		{
// 			str[_i] = DOUBLE_QUOTE;
// 			*quote = 0;
// 			if (str[_i + 1] == '\'')
// 				*quote = 1;
// 			_i++;//to include the DOUBLE_QUOTE
// 			*i += _i;
// 			break;
// 		}
// 		else if (str[_i] == '$')
// 		{
// 			if (is_valid_key_char(str[_i + 1], 1) || 
// 				(str[_i + 1] >= '0' && str[_i + 1] <= '9') || 
// 				str[_i + 1] == '?' || str[_i + 1] == '$')
// 			{
// 				*i += _i;
// 				break;
// 			}
// 		}
// 		_i++;
// 	}
// 	*data =ft_substr(str, 0, _i);
// 	if (!*data)
// 		return (perror("error: "), R_FAIL);
// 	return (R_SUCCESS);
// }

// int	expand_double_quote(t_expand_funcs_data e)
// {
// 	printf("DOUBLE\n");
// 	int join;

// 	join = 0;
// 	printf("------------start%d&------------\n",*e.i);
// 	if (extract_word_of_double_quote(e.str + *e.i, e.i, e.data, e.quote) == R_FAIL)
// 		return (free_expand_tokens_list(*e.tokens), R_FAIL);

// 	printf("---------------&%d&------------\n",*e.i);
// 	if (e.str[*(e.i)] && e.str[*(e.i) + 1] != ' ')
// 		join = 1;
// 	if (append_expand_token(e.tokens, *e.data, 1, *e.quote, join) == R_FAIL)
// 		return (free_expand_tokens_list(*e.tokens), R_FAIL);
// 	printf("-------------out%d&------------\n",*e.i);
// 	return (R_SUCCESS);
// }


/*
// Mocked functions	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');

// Prototype
static int extract_word_of_double_quote(char *str, int *i, char **data);

// Test function
void test_extract_word_of_double_quote()
{
	int i;
	char *data = NULL;

	// Test 1: Basic double quote ending
	char input1[] = "\"hello world\" rest";
	i = 0;
	if (extract_word_of_double_quote(input1, &i, &data) == R_SUCCESS)
	{
		if (strcmp(data, "\x0Fhello world\x0F") == 0)
			printf("Test 1 passed\n");
		else
		{
			printf("Test 1 failed: [%s]{%d}\n", data, strlen(data));
			int i = -1;
			while (*(data + ++i))
			{
				printf("[%d]", data[i]);
			}
			printf("\n");
		}
		free(data);
	}
	else
		printf("Test 1 failed: function returned R_FAIL\n");

	
	// Test 2: Stop at variable
	char input2[] = "\"home $USER\"";
	i = 0;
	if (extract_word_of_double_quote(input2, &i, &data) == R_SUCCESS)
	{
		if (memcmp(data, "\x0Fhome ", strlen("\x0Fhome ")) == 0)
			printf("Test 2 passed\n");
		else
			printf("Test 2 failed: [%s]\n", data);
		free(data);
	}
	else
		printf("Test 2 failed: function returned R_FAIL\n");

	
	// Test 3: No closing quote
	char input3[] = "\"unterminated quote test";
	i = 0;
	if (extract_word_of_double_quote(input3, &i, &data) == R_SUCCESS)
	{
		if (memcmp(data, "\x0Funterminated quote test", strlen("\x0Funterminated quote test")) == 0)
			printf("Test 3 passed\n");
		else
			printf("Test 3 failed: [%s]\n", data);
		free(data);
	}
	else
		printf("Test 3 failed: function returned R_FAIL\n");

	// Test 4: Only double quotes
	char input4[] = "\"\"";
	i = 0;
	if (extract_word_of_double_quote(input4, &i, &data) == R_SUCCESS)
	{
		if (memcmp(data, "\x0F\x0F", 2) == 0)
			printf("Test 4 passed\n");
		else
			printf("Test 4 failed: [%s]\n", data);
		free(data);
	}
	else
		printf("Test 4 failed: function returned R_FAIL\n");
}

int main()
{
	test_extract_word_of_double_quote();
	return 0;
}
*/
