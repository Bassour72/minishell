
#include "../include/parsing.h"

// static char *typetostring[] = {
// 	[OP_OR] = "OR",
// 	[OP_AND] = "AND",
// 	[PIPE] = "PIPE",
// 	[BLOCK] = "BLOCK"
// };

int	create_token(t_token *token, char *data, int type)
{
	token->data = ft_strdup(data);
	if (!token->data)
		return (perror("error: "), R_FAIL);
	token->type = type;
	token->is_listed = 0;
	// printf("%s\n", typetostring[token->type]);
	return (R_SUCCESS);
}

void free_tokens_list(t_token *tokens) 
{
	int i;
	t_token	*list;

	if (!tokens)
		return ;
	list = tokens;
	i = -1;
	while ((list + ++i)->data)
	{
		// printf("%p\n", list + i);
		free((list + i)->data);
	}
	free(tokens);
	// *tokens = NULL;
}
void free_splitted_input(t_node *splitted_inpt)
{
	t_node *tmp;

	while (splitted_inpt)
	{
		tmp = splitted_inpt;
		splitted_inpt = splitted_inpt->next;
		free(tmp->data);
		free(tmp);
	}
}

int count_splitted_input_nodes(t_node *splitted_input)
{
	int count;

	count = 0;
	while (splitted_input)
	{
		count++;
		splitted_input = splitted_input->next;
	}
	return (count);
}

int convert_inputs_to_tokens(t_token *tokenized_input, t_node *splitted_input)// todo protect all functions
{
	int i = 0;
	int state;
	
	i = 0;
	state = R_SUCCESS;
	while (splitted_input)
	{
		if (ft_strncmp(splitted_input->data, "&&", 2) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, OP_AND);

		else if (ft_strncmp(splitted_input->data, "||", 2) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, OP_OR);
			
		else if (ft_strncmp(splitted_input->data, "|", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, PIPE);	//need to free memory

		else if (ft_strncmp(splitted_input->data, ">>", 2) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, RED_APPEND);

		else if (ft_strncmp(splitted_input->data, "<<", 2) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, HER_DOC);
		
		else if (ft_strncmp(splitted_input->data, "<", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, RED_INPUT);
		
		else if (ft_strncmp(splitted_input->data, ">", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, RED_TRUNK);

		else if (ft_strncmp(splitted_input->data, "(", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, PAREN_OPEN);
		else if (ft_strncmp(splitted_input->data, ")", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, PAREN_CLOSE);
		//if the input before this one is a redir so this one will be a filename
			


		else if (ft_strncmp(splitted_input->data, "*", 1) == 0)
			state = create_token(tokenized_input + i, splitted_input->data, WILD_CARD);
			
		else if (i != 0 && (tokenized_input[i - 1].type == RED_APPEND || 
							tokenized_input[i - 1].type == RED_TRUNK || 
							tokenized_input[i - 1].type == RED_INPUT || 
							tokenized_input[i - 1].type == HER_DOC))
			state = create_token(tokenized_input + i, splitted_input->data, T_FILE_NAME);
		else
			state = create_token(tokenized_input + i, splitted_input->data, WORD);
			i++;
		if (state == R_FAIL)
			return (R_FAIL);
		splitted_input = splitted_input->next;
	}
	(tokenized_input + i)->data = NULL;
	return (R_SUCCESS);
}

int tokenizer(t_token **tokenized_input, char *input)
{
	// char	**splitted_input; // need to be freeied
	int		i;
	
	t_node *splitted_input;
	
	splitted_input = NULL;
	if (split(&splitted_input, input) == R_FAIL)
		return (free(input), R_FAIL);
		
	free(input); //todo find a better place for this
	// t_node *s = splitted_input;
	// while (s)
	// {
	// 	printf("<%s>\n", s->data);
	// 	s = s->next;
	// }
	
	if (!splitted_input)
		return (R_FAIL);
	// free_splitted_input(splitted_input);
	// return (R_SUCCESS);
	//alocate space for the tokenized inputs
	*tokenized_input = malloc(sizeof(t_token) * (count_splitted_input_nodes(splitted_input) + 1));
	if (!*tokenized_input)
	{
		perror("error: ");
		free_splitted_input(splitted_input);
		return (R_FAIL);
	}

	if (convert_inputs_to_tokens(*tokenized_input, splitted_input) == R_FAIL)
		return (free_splitted_input(splitted_input), free_tokens_list(*tokenized_input), R_FAIL); // maybe free *tokenized_input

	free_splitted_input(splitted_input);

	return (R_SUCCESS);
}
