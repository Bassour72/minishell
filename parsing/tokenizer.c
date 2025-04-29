/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: massrayb <massrayb@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:08:10 by massrayb          #+#    #+#             */
/*   Updated: 2025/04/29 14:50:53 by massrayb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (0); //strdup failed 
	token->type = type;
	token->is_listed = 0;
	// printf("%s\n", typetostring[token->type]);
	return (1); //success
}

void free_tokens_list(t_token *tokens, int len) //TODO :finish later
{
	int i;

	i = -1;
	while (++i < len)
	{
		free((tokens + i)->data);
		
	}
}

t_token	*tokenizer(char *input)
{
	// char	**splitted_input; // need to be freeied
	t_token	*tokenized_input;
	int		i;
	
	t_node *splitted_input = split(input);
	// splitted_input = ft_split2(input); // just for testing
	
	//alocate space for the tokenized inputs
	i = 0; 
	t_node *t = splitted_input;
	while (t)
	{
		i++;
		t = t->next;
	}
		// while (*(splitted_input + i) != NULL)//here im using i to calculate the tokens count
		// i++;
	tokenized_input = malloc(sizeof(t_token) * (i + 1));
	if (!tokenized_input)
		return (NULL);
	i = 0;
	while (splitted_input)
	{
		printf("splitted_inpt = %s\n",splitted_input->data);
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
			
		else if (ft_strncmp(splitted_input->data, "\"", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, QUOTE_DOUBLE);
		else if (ft_strncmp(splitted_input->data, "\'", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, QUOTE_SINGLE);
			
		else if (ft_strncmp(splitted_input->data, "*", 1) == 0)
			create_token(tokenized_input + i, splitted_input->data, WILD_CARD);
			
		else
			create_token(tokenized_input + i, splitted_input->data, WORD);
			i++;
		splitted_input = splitted_input->next;
	}
	printf("i = %d\n", i);
	
	(tokenized_input + i)->data = NULL;
	
	// printf("done splitting %d tokens\n", i);
	return (tokenized_input);
}

	//convert each input from splited_input into a token
	// i = -1;
	// while (splitted_input[++i])
	// {
	// 	if (ft_strncmp(*(splitted_input + i), "&&", 2) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), OP_AND);

	// 	else if (ft_strncmp(*(splitted_input + i), "||", 2) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), OP_OR);
			
	// 	else if (ft_strncmp(*(splitted_input + i), "|", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), PIPE);	//need to free memory

	// 	else if (ft_strncmp(*(splitted_input + i), ">>", 2) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), RED_APPEND);

	// 	else if (ft_strncmp(*(splitted_input + i), "<<", 2) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), HER_DOC);
		
	// 	else if (ft_strncmp(*(splitted_input + i), "<", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), RED_INPUT);
		
	// 	else if (ft_strncmp(*(splitted_input + i), ">", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), RED_TRUNK);

	// 	//if the input before this one is a redir so this one will be a filename
	// 	else if (i != 0 && (tokenized_input[i - 1].type == RED_APPEND || 
	// 						tokenized_input[i - 1].type == RED_TRUNK || 
	// 						tokenized_input[i - 1].type == RED_INPUT || 
	// 						tokenized_input[i - 1].type == HER_DOC))
	// 		create_token(tokenized_input + i, *(splitted_input + i), T_FILE_NAME);
			
	// 	else if (ft_strncmp(*(splitted_input + i), "(", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), PAREN_OPEN);
	// 	else if (ft_strncmp(*(splitted_input + i), ")", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), PAREN_CLOSE);
			
	// 	else if (ft_strncmp(*(splitted_input + i), "\"", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), QUOTE_DOUBLE);
	// 	else if (ft_strncmp(*(splitted_input + i), "\'", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), QUOTE_SINGLE);
			
	// 	else if (ft_strncmp(*(splitted_input + i), "*", 1) == 0)
	// 		create_token(tokenized_input + i, *(splitted_input + i), WILD_CARD);
			
	// 	else
	// 		create_token(tokenized_input + i, *(splitted_input + i), WORD);
	// }