#include "../../include/execution.h"

int has_no_newline_option(t_tree *root, int *start_index)
{
    int arg_idx = 1;
    int char_idx;

    if (!root || !root->data)
        return (0);
    while (root->data[arg_idx])
    {
        if (root->data[arg_idx][0] != '-')
            break;
        char_idx = 1;
        while (root->data[arg_idx][char_idx] == 'n')
            char_idx++;
        if (root->data[arg_idx][char_idx] == '\0' && char_idx > 1)
            arg_idx++;
        else
            break;
    }
    *start_index = arg_idx;
    return (arg_idx > 1);
}

int builtin_echo(t_tree *root)
{
    int arg_idx;
    int suppress_newline = 0;

    if (!root || !root->data || !root->data[0])
        return (1);
    if (has_no_newline_option(root, &arg_idx))
        suppress_newline = 1;
    while (root->data[arg_idx])
    {
        write(STDOUT_FILENO, root->data[arg_idx], ft_strlen(root->data[arg_idx]));
        if (root->data[arg_idx + 1])
            write(1, " ", 1);
        arg_idx++;
    }
    if (!suppress_newline)
        write(1, "\n", 1);
    return (0);
}