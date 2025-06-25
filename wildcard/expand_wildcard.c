#include "../include/wildcard.h"

static int is_wildcard(char *str)
{
	int literal_string;

	literal_string = -1;
    while (*str)
    {
		if (*str == DOUBLE_QUOTE || *str == SINGLE_QUOTE)
			literal_string = -literal_string;
        if (*str == '*' && literal_string == -1)
            return (1);
        str++;
    }
    return (0);
}

int init_needle_len(char *name, int start)
{
    int len;
    int i;

    len  = 0;
    i = start - 1;
    while (name[++i] && name[i] != '*')
        len++; 
    // printf("len:%d\n", len);

    return (len);
}
/*
int is_file_name_match(char *file_name, char *str)
{
    int wc_pos;			// abcd.c
    int  i;			// *a
    int j;
	int file_name_len = ft_strlen(file_name);
	int str_len = ft_strlen(str);
	int is_literal_string = -1;

	wc_pos = -1;
    j = 0;
    i = 0;
	// printf("filename >>>>  [%s]\n", file_name);
	if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
		i++;
	while (str[i] == file_name[j])
	{
		(i++, j++);
		if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
			i++;
	}
	while (1)
	{
		printf("[i: %d]\n", i);
		if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
		{
			printf("#######################\n");
			i++;
			is_literal_string = -is_literal_string;
		}
		if (is_literal_string != 1)
		{
			printf("not litral-------------------------\n");
			while (str[i] == '*')
				wc_pos = ++i;
		}
		else
			i++;
		
		if (str[i] == file_name[j])
		{
			printf(">--1--[%c][%c]----{%s}{%s}<\n", str[i], file_name[j], file_name, str);
			(i++, j++);
			if (i == str_len && j == file_name_len)
				return (1);
			continue;
		}
		else
		{
			printf(">--2--[%c][%c]----{%s}{%s}<\n", str[i], file_name[j], file_name, str);
			if (wc_pos != i)
				i = wc_pos;
			else
				j++;
			if (j == file_name_len && wc_pos == i && i != str_len)
				return (0);
			else if (j == file_name_len && i == str_len)
				return (1);
			continue;
		}
    }
	   
}
*/

int is_file_name_match(char *file_name, char *str)
{
    int wc_pos;			// abcd.c
    int	i;			// *a
    int j;
	int file_name_len = ft_strlen(file_name);
	int str_len = ft_strlen(str);
	int is_literal_string = -1;

	wc_pos = -1;
    j = 0;
    i = 0;
	// while (str[i])
	// {
	// 	if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
	// 	{
	// 		printf("-quote found i:%d[%c] | j:%d[%c]\n", i, j, str[i], file_name[j]);												
	// 		(i++, is_literal_string *= -1);
	// 	}
	// 	if (str[i] == '*' && is_literal_string == -1)
	// 	{
	// 		i++;
	// 		break;
	// 	}
	// 	i++;
	// }
	// if (str[i])
	// printf("\n\nfilename >>>>  [%s] [%s]\n",str, file_name);
	while (1)
	{
		if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
		{
			printf("-quote found i:%d[%c] | j:%d[%c]\n", i, j, str[i], file_name[j]);												
			(i++, is_literal_string *= -1);
			
		}
		else if (str[i] == '*' && is_literal_string == -1)
		{
			printf("-wc found i:%d[%c] | j:%d[%c]\n", i, j, str[i], file_name[j]);
			{
				wc_pos = ++i;
				if (str[i] == SINGLE_QUOTE || str[i] == DOUBLE_QUOTE)
					wc_pos = i + 1;
			}										
		}
		
		else if (str[i] != file_name[j])
		{
			printf("-no match i:%d[%c] | j:%d[%c]\n", i, j, str[i], file_name[j]);
			if (wc_pos != i)
				i = wc_pos;
			else
				j++;							
			if ((j == file_name_len && wc_pos == i && i != str_len) || wc_pos == -1)
				return (0);
			else if (j == file_name_len && i == str_len)
				return (1);
		}
		else
		{
			printf("-match i:%d[%c] | j:%d[%c]\n", i, j, str[i], file_name[j]);											
			if (i == str_len && j == file_name_len)
				return (1);
			(i++, j++);
		}
		if (j == file_name_len && i == str_len)
			return (1);
		// sleep(1);
    }
	   
}

void init_file_names_matches(t_wc_node *file_names, char *name)
{
    while (file_names)
    {
        if (is_file_name_match(file_names->data, name) == 1)
            file_names->match = 1;
        file_names = file_names->next;
    }
}

int do_expand_wildcard(t_wc_node **wc_node, t_wc_node *file_names)
{
	t_wc_node *new_list = NULL;

    init_file_names_matches(file_names, (*wc_node)->data);
	
	int len = 0;
	for(t_wc_node *fn = file_names; fn; fn = fn->next)
		if (fn->match) len++;

	if (len == 0)
	{
		free((*wc_node)->data);
		free(*wc_node);
		*wc_node = NULL;
	}
	else
	{
		for(t_wc_node *fn = file_names; fn; fn = fn->next)
		{
			if (fn->match)
			{
				if (append_wc_node(&new_list, fn->data) == R_FAIL)
					return (free_wc_node_list(new_list), *wc_node = NULL, R_FAIL);
			}
		}
		free((*wc_node)->data);
		free(*wc_node);
		*wc_node = new_list;
	}
	return (R_SUCCESS);
}

int expand_wildcard(t_wc_node **args_list, t_wc_node *file_names)
{
    t_wc_node *tmp;
    t_wc_node *wc_node;
	t_wc_node *befor;
	t_wc_node *after;

    tmp = *args_list;
    while (tmp)
    {
		// printf("[%s]\n", tmp[1].data);
        wc_node = tmp;
        tmp = tmp->next;
        if (is_wildcard(wc_node->data))
        {
			// printf("\033[33m[%s]\033[0m is a wildcard with lenght \033[33m[%d]\033[0m\n", wc_node->data, ft_strlen(wc_node->data));
			befor = wc_node->prev;
			after = wc_node->next;
            if (do_expand_wildcard(&wc_node, file_names) == R_FAIL)
				return (R_FAIL);
			if(!wc_node)
			{
				if (befor)
					befor->next = after;
				if (after)
					after->prev = befor;
			}
			else
			{ 
				if (befor)
					befor->next = wc_node;
				else
					*args_list = wc_node;
				while (wc_node->next)
					wc_node = wc_node->next;
				if(after)
					after->prev = wc_node;
				wc_node->next = after;
			}
        }
    }
	return (R_SUCCESS);
	// for (t_wc_node *t = *args_list; t; t = t->next)
	// 	printf(">>{%s}\n", t->data);

}
