#include "../../../include/parsing.h"

void free_expand_tokens_list(t_expand_token *tokens)
{
    t_expand_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->data);
        free(tmp);
    }
}