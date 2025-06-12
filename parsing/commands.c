
#include "../include/parsing.h"

void free_cmd_list(char **old_list)
{
	int i;

	i = -1;
	while (old_list[++i])
		free(old_list[i]);
	free(old_list);
}

char **append_command(char **old_list, char *new_cmd)//checked
{
	int	size;
	int	i;
	int	j;
	char **new_list;

	if (!new_cmd) //todo maybe i dont need this
		return (printf("Error: check append command |temp|\n"), NULL);
	//LABLE : build new cmd list from 0
	if (!old_list)
	{
		new_list = malloc(sizeof(char *) * 2);
		if (!new_list)
			return (perror("error: "), NULL);
		new_list[0] = ft_strdup(new_cmd);
		if (new_list[0] == NULL)
			return (perror("error: "), free(new_list), NULL);
		*(new_list + 1) = NULL;
		return (new_list);
	}
	//lable : fill new list with old list + new cmd
	size = 0;
	while (old_list[size++])
		;
		
	new_list = malloc(sizeof(char *) * (size + 1));
	if (!new_list)
		return (perror("error: "), free_cmd_list(old_list), NULL); //note free old list
		
	i = -1;
	while (old_list[++i]) 
	{
		new_list[i] = ft_strdup(old_list[i]);
		if (!new_list[i]) //note free old + new list 
		{	
			free_cmd_list(old_list);
			return (perror("error: "), free_cmd_list(new_list), NULL);
		}
	}

	free_cmd_list(old_list);//note don't need old_list anymore

	new_list[i] = ft_strdup(new_cmd);
	if (new_list[i] == NULL)
		return (perror("error: "), free_cmd_list(new_list), NULL);
	new_list[i + 1] = NULL;
	return (new_list);
}
