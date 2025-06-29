#include "../include/wildcard.h"

int	get_wc_list_len(t_wc_node *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}

void	cleanup_wc_list_shels(t_wc_node *list)
{
	t_wc_node *tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

void assign_wc_node_data_to_args_array(t_wc_node *list, char ***arr)
{
	int	i;

	i = 0;
	while (list)
	{
		(*arr)[i++] = list->data;
		list = list->next;
	}
	(*arr)[i] = NULL;
}

// int is_file_name_match(char *file_name, char *str)
// {
//     int wc_pos = -1;
//     int i = 0;
//     int j = 0;
//     int file_name_len = ft_strlen(file_name);
//     int str_len = ft_strlen(str);
//     int is_literal_string = -1;

//     while (1)
//     {
//         if (str[i] == DOUBLE_QUOTE || str[i] == SINGLE_QUOTE)
//             (i++, is_literal_string *= -1);

//         else if (str[i] == '*' && is_literal_string == -1)
//         {
//             wc_pos = ++i;
//             while (str[wc_pos] == DOUBLE_QUOTE || str[wc_pos] == SINGLE_QUOTE)
//                 wc_pos++;  // Skip quotes after *

//             int suffix_len = str_len - wc_pos;

//             // Make sure suffix exists
//             if (suffix_len == 0)
//                 return (1); // pattern was just "*", so all filenames match

//             // Try to find the suffix at the right position
//             while (file_name[j])
//             {
//                 if (ft_strncmp(file_name + j, str + wc_pos, suffix_len) == 0)
//                 {
//                     // Special case for "*.*"
//                     if (ft_strcmp(str, "*.*") == 0)
//                     {
//                         if (j == 0 || j + suffix_len == file_name_len)
//                             return (0); // dot is at start or end
//                     }
//                     return (1);
//                 }
//                 j++;
//             }
//             return (0);
//         }

//         else if (str[i] != file_name[j])
//         {
//             if (wc_pos != i)
//                 i = wc_pos;
//             else
//                 j++;
//             if ((j == file_name_len && wc_pos == i && i != str_len) || wc_pos == -1)
//                 return (0);
//             else if (j == file_name_len && i == str_len)
//                 return (1);
//         }
//         else
//         {
//             if (i == str_len && j == file_name_len)
//                 return (1);
//             (i++, j++);
//         }

//         if (j == file_name_len && i == str_len)
//             return (1);
//     }
// }