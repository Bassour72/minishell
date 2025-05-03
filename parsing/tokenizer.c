
#include "../include/parsing.h"

static char *typetostring[] = {
	[OP_OR] = "OR",
	[OP_AND] = "AND",
	[PIPE] = "PIPE",
	[BLOCK] = "BLOCK"
};

int	create_token(t_token *token, char *data, int type)
{
	token->data = ft_strdup(data);
	if (!token->data)
		return (0);
	token->type = type;
	token->is_listed = 0;
	// printf("%s\n", typetostring[token->type]);
	return (1);
}

void free_tokens_list(t_token *tokens) 
{
	int i;
	t_token	*list;

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

void convert_inputs_to_tokens(t_token *tokenized_input, t_node *splitted_input)
{
	int i = 0;
	
	i = 0;
	while (splitted_input)
	{
		// printf("splitted_inpt = %s\n",splitted_input->data);
		if (ft_strncmp(splitted_input->data, "&&", 2) == 0)
			create_token(tokenized_input + i, splitted_input->data, OP_AND);

		else if (ft_strncmp(splitted_input->data, "||", 2) == 0)
			create_token(tokenized_input + i, splitted_input->data, OP_OR);
			
		else if (ft_strncmp(splitted_input->data, "|", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, PIPE);	//need to free memory

		else if (ft_strncmp(splitted_input->data, ">>", 2) == 0)
			create_token(tokenized_input + i, splitted_input->data, RED_APPEND);

		else if (ft_strncmp(splitted_input->data, "<<", 2) == 0)
			create_token(tokenized_input + i, splitted_input->data, HER_DOC);
		
		else if (ft_strncmp(splitted_input->data, "<", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, RED_INPUT);
		
		else if (ft_strncmp(splitted_input->data, ">", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, RED_TRUNK);

		//if the input before this one is a redir so this one will be a filename
		else if (i != 0 && (tokenized_input[i - 1].type == RED_APPEND || 
							tokenized_input[i - 1].type == RED_TRUNK || 
							tokenized_input[i - 1].type == RED_INPUT || 
							tokenized_input[i - 1].type == HER_DOC))
			create_token(tokenized_input + i, splitted_input->data, T_FILE_NAME);
			
		else if (ft_strncmp(splitted_input->data, "(", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, PAREN_OPEN);
		else if (ft_strncmp(splitted_input->data, ")", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, PAREN_CLOSE);
			
		// else if (ft_strncmp(splitted_input->data, "\"", 1) == 0)
		// 	create_token(tokenized_input + i, splitted_input->data, QUOTE_DOUBLE);
		// else if (ft_strncmp(splitted_input->data, "\'", 1) == 0)
		// 	create_token(tokenized_input + i, splitted_input->data, QUOTE_SINGLE);
			
		else if (ft_strncmp(splitted_input->data, "*", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, WILD_CARD);
			
		else
			create_token(tokenized_input + i, splitted_input->data, WORD);
			i++;
		splitted_input = splitted_input->next;
	}
	(tokenized_input + i)->data = NULL;
}

t_token	*tokenizer(char *input)
{
	// char	**splitted_input; // need to be freeied
	t_token	*tokenized_input;
	int		i;
	
	t_node *splitted_input = split(input);//todo check for all cases
	free(input); //todo find a better place for this

	// t_node *s = splitted_input;
	// while (s)
	// {
	// 	printf("<%s>\n", s->data);
	// 	s = s->next;
	// }

	if (!splitted_input)
		return (NULL);

	//alocate space for the tokenized inputs
	tokenized_input = malloc(sizeof(t_token) * (count_splitted_input_nodes(splitted_input) + 1));
	if (!tokenized_input)
	{
		printf("error: %s\n", strerror(errno));//todo this is tmp
		free_splitted_input(splitted_input);
		return (NULL);
	}

	convert_inputs_to_tokens(tokenized_input, splitted_input);
	
	free_splitted_input(splitted_input);


	return (tokenized_input);
}
