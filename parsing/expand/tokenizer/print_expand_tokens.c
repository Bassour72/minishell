#include "../../../include/parsing.h"

void print_expand_tokens(t_expand_token *tokens)
{
    printf("======================================\n");
    while (tokens)
    {
        printf("data: [\033[33m%s\033[0m], splitable: [%d], join: [%d]\n", tokens->data, tokens->split, tokens->join);
        tokens = tokens->next;
    }
    printf("======================================\n");
}