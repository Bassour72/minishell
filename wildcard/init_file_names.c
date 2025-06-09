#include "../include/wildcard.h"

static char *init_cwd()
{
    char *cwd;
    // char *tmp;

    cwd = malloc(PATH_MAX);
    if (!cwd)
        return (perror("error: "), NULL);
    // tmp = cwd;
    if (!getcwd(cwd, PATH_MAX))
    {
        perror("error: ");
        free(cwd);
        return (NULL);
    }
    return (cwd);
}

int append_wc_node(t_wc_node **list, char *data)
{
    t_wc_node *node;
    t_wc_node *tmp;
    char *new_data;

    new_data = ft_strdup(data);
    if (!new_data)
        return (perror("error: "), R_FAIL);
    node = malloc(sizeof(t_wc_node));
    if (!node)
        return (perror("error: "), free(new_data), R_FAIL);
    node->data = new_data;
    node->match = 0;
    node->next = NULL;
	node->prev = NULL;
    if (!*list)
        *list = node;
    else
    {
        tmp = *list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
		node->prev = tmp;
    }
    return (R_SUCCESS);
}

int init_file_names(t_wc_node **file_names)
{
    char    *cwd;
    DIR     *dir;
    struct dirent *dir_entry;

    cwd = init_cwd();
    if (!cwd)
        return (R_FAIL);
    dir = opendir(cwd);
    if (!dir)
        return (perror("error: "), free(cwd), R_FAIL);
    errno = 0;
    while (errno == 0)
    {
        dir_entry = readdir(dir);
        if (!dir_entry)
            break ;
        if (append_wc_node(file_names, dir_entry->d_name) == R_FAIL)
            return (free_wc_node_list(*file_names), closedir(dir), free(cwd), R_FAIL);
    }
	if (dir)
    	closedir(dir);
    if (errno != 0)
        return (perror("error: "),free_wc_node_list(*file_names), free(cwd), R_FAIL);
    return (free(cwd), R_SUCCESS);
}
