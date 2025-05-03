

#ifndef PARSER_H
#define PARSER_H

//includes ----------------------------------------------------------
# include "../_libft/libft.h"
# include <stdio.h>
# include <string.h>
#include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
//-------------------------------------------------------------------

//node types --------------------------------------------------------

typedef enum e_type
{
	T_FILE_NAME,			//file.txt
	WORD,				// cat, ls,...
	PIPE, 				// |
	RED_INPUT,  	// <
	RED_APPEND, // >
	RED_TRUNK,	// >>
	HER_DOC,  			// <<
	BLOCK, 
	OP_AND,
	OP_OR,
	PAREN_OPEN,
	PAREN_CLOSE,
	WILD_CARD,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}			t_type;

// #define RED_OUT_TRUNK 5
// #define RED_OUT_APPND 6
// #define HERDOC 7
// 
//-------------------------------------------------------------------

typedef struct s_node
{
	char *data;
	struct s_node *next;
}	t_node;

typedef struct s_red
{
	t_type			type;
	char			*data; //note file name or herdoc
	struct s_red	*next;
}	t_red;

typedef struct s_token
{
	char	*data;
	t_type	type;
	int		is_listed;
}				t_token;

typedef struct s_tree
{
	int				empty; 	// 0:cmd | 1:(  | -1:)
	char 			**data;
	char			*file_name; //may also contain her-doc EOF | PS:contact prismo
	t_type			type;
	t_red			*redirections;
	// struct s_tree	*parent;
	struct s_tree	*left;
	struct s_tree	*right;
}				t_tree;

typedef struct s_flat_tree
{
	t_tree *tree_node;
	struct s_flat_tree *prev;
	struct s_flat_tree *next;
}				t_flat_tree;

// typedef enum e_node_type
// {
// 	GROUP, CMD, PIPE, OPERATOR
// } 	t_node_type;


// typedef struct	s_node
// {
// 	t_node_type		type;
// 	t_red			*red;
// 	char			**cmd;
// 	struct s_node	*left;
// 	struct s_node	*right;

// }	t_node;

//split.c
t_node *split(char *str);


//parser.c
t_tree	*parser(t_tree *tree, char *input);
void free_tree(t_tree *root);

//tokenizer.c
t_token	*tokenizer(char *input);
void free_tokens_list(t_token *tokens);

//tree
t_flat_tree	*create_flat_tree(t_token *tokens);
t_tree	build_the_tree(t_tree *tree, t_tree *flat_tree);
int		get_next_root(t_tree *flat_tree, int len, int side);

//p_commands
char **append_command(char **old_list, char *new_cmd);

//redirection
void append_redirection(t_red *redirections_list, char *data, t_type type);

#endif