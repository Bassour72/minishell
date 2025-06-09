#include "../include/wildcard.h"

static int is_wildcard(char *str)
{
    while (*str)
    {
        if (*str == '*')
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

int is_file_name_match(char *file_name, char *str)
{
    int wc;			// abcd.c
    int  i;			// *a
    int j;
	int file_name_len = ft_strlen(file_name);
	int str_len = ft_strlen(str);

	wc = -1;
    j = 0;
    i = 0;
	// printf("filename >>>>  [%s]\n", file_name);
	while (str[i] == file_name[j])
		(i++, j++);
	if(str[i] != '*')
		return (0);

	while (1)
	{
		while (str[i] == '*')
			wc = ++i;
		if (str[i] == file_name[j])
		{
			// printf(">--1--<\n");
			(i++, j++);
			if (i == str_len && j == file_name_len)
				return ( 1);
			continue;
		}
		else
		{
			// printf(">--2--<\n");
			if (wc != i)
				i = wc;
			else
				j++;
			if (j == file_name_len && wc == i && i != str_len)
				return (0);
			else if (j == file_name_len && i == str_len)
				return (1);
			continue;
		}
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
					return ( free_node_list(new_list), *wc_node = NULL, R_FAIL);
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
        wc_node = tmp;
        tmp = tmp->next;
        if (is_wildcard(wc_node->data))
        {
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
