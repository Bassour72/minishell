#include "../include/parsing.h"

static int	convert_operators(t_token *tokenized_input, t_node *splitted_input, \
	int i)
{
	if (ft_strncmp(splitted_input->data, "&&", 2) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			OP_AND));
	else if (ft_strncmp(splitted_input->data, "||", 2) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, OP_OR));
	else if (ft_strncmp(splitted_input->data, "|", 1) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, PIPE));
	return (R_CONTINUE);
}

static int	convert_reds(t_token *tokenized_input, t_node *splitted_input, \
	int i)
{
	if (ft_strncmp(splitted_input->data, ">>", 2) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			RED_APPEND));
	else if (ft_strncmp(splitted_input->data, "<<", 2) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			HER_DOC));
	else if (ft_strncmp(splitted_input->data, "<", 1) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			RED_INPUT));
	else if (ft_strncmp(splitted_input->data, ">", 1) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			RED_TRUNK));
	else if (i != 0 && (tokenized_input[i - 1].type == RED_APPEND || \
						tokenized_input[i - 1].type == RED_TRUNK || \
						tokenized_input[i - 1].type == RED_INPUT || \
						tokenized_input[i - 1].type == HER_DOC))
		return (create_token(tokenized_input + i, splitted_input->data, \
			T_FILE_NAME));
	return (R_CONTINUE);
}

static int	conver_parenths(t_token *tokenized_input, t_node *splitted_input, \
	int i)
{
	if (ft_strncmp(splitted_input->data, "(", 1) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			PAREN_OPEN));
	else if (ft_strncmp(splitted_input->data, ")", 1) == 0)
		return (create_token(tokenized_input + i, splitted_input->data, \
			PAREN_CLOSE));
	return (R_CONTINUE);
}

int	convert_inputs_to_tokens(t_token *tokenized_input, t_node *splitted_input)
{
	int	i;
	int	state;

	i = 0;
	state = R_SUCCESS;
	while (splitted_input)
	{
		state = 0;
		state = convert_operators(tokenized_input, splitted_input, i);
		if (state == R_CONTINUE)
			state = conver_parenths(tokenized_input, splitted_input, i);
		if (state == R_CONTINUE)
			state = convert_reds(tokenized_input, splitted_input, i);
		if (state == R_CONTINUE)
			state = create_token(tokenized_input + i, splitted_input->data, \
				WORD);
		if (state == R_FAIL)
			return (R_FAIL);
		i++;
		splitted_input = splitted_input->next;
	}
	(tokenized_input + i)->data = NULL;
	return (R_SUCCESS);
}

int	tokenizer(t_token **tokenized_input, char *input)
{	
	t_node	*splitted_input;

	splitted_input = NULL;
	if (split(&splitted_input, input) == R_FAIL)
		return (free(input), R_FAIL);
	free(input);
	if (!splitted_input)
		return (R_FAIL);
	*tokenized_input = malloc(sizeof(t_token) * \
	(count_splitted_input_nodes(splitted_input) + 1));
	if (!*tokenized_input)
	{
		perror("error: ");
		free_splitted_input(splitted_input);
		return (R_FAIL);
	}
	if (convert_inputs_to_tokens(*tokenized_input, splitted_input) == R_FAIL)
	{
		free_splitted_input(splitted_input);
		free_tokens_list(*tokenized_input);
		return (R_FAIL);
	}
	free_splitted_input(splitted_input);
	return (R_SUCCESS);
}
