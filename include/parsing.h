#ifndef PARSER_H
#define PARSER_H

//includes ----------------------------------------------------------
# include "../_libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include "env.h"
# include "expand.h"
# include "wildcard.h"
# include <termios.h>


//-------------------------------------------------------------------

#include <assert.h> //just for debugging

//node types --------------------------------------------------------

typedef enum e_type
{
	T_FILE_NAME,			//file.txt
	WORD,				// cat, ls,...
	PIPE, //				// |
	RED_INPUT,  	// <
	RED_APPEND, // >>
	RED_TRUNK,	// >
	HER_DOC,  			// <<
	BLOCK,//
	OP_AND,//
	OP_OR,//
	PAREN_OPEN,
	PAREN_CLOSE,
	WILD_CARD,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}			t_type;

//-------------------------------------------------------------------

extern const char *typetostring[];


typedef struct s_red
{
	t_type			type;
	int				save_stdout;
	bool			is_has_stdout;
	char			*target;
	char			*heredoc_path;
	int				in_fd;
	int				out_fd;
	int				pipe[2];
	char			*data; //note file name or herdoc 
	int				is_ambiguous;
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
	int				empty;
	char 			**data;
	t_type			type;
	t_red			*redirections;
	int pipe_out; //for execution 
	int out_fd;
	int is_forked;
	struct s_tree	*left;
	struct s_tree	*right;
}				t_tree;

typedef struct s_flat_tree
{
	t_tree *tree_node;
	struct s_flat_tree *prev;
	struct s_flat_tree *next;
	struct s_flat_tree *backup_next;
}				t_flat_tree;


int	init_env(t_env **env_list); //  for minishell !!
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
int			split(t_node **list, char *str);
int			is_special(char *c);
int			append_node(t_node **list, char **data);
int			m_operators(t_node **list, char *str, int *i);
int			m_parenth(t_node **list, char *str, int *i);
int			m_redirections(t_node **list, char *str, int *i);
int			m_quotes( char *str, char **data, int *i);
int			m_normal(char *str, char **data, int *i);
int			m_expand_normal(char *str, char **data, int *i);
int			expand_split2(t_node **list, char *str);
void		skip_spaces(char *str, int *i);
void		split_cleaner(t_node *list);




//parser.c
int			parser(t_tree **tree, char *input, t_env **env);
void		free_tree(t_tree *root);
void		print_tree(t_tree *root, int i);

//lable parsing
	//tree
t_tree		*new_tree_node(t_type type);
t_tree		*init_tree(t_flat_tree *ft);
	// dbugging
void		print_tokenized_inputs(t_token *tokenized_input);
void		print_tree(t_tree *root, int i);



	//clear_parsing
void		free_tree_node(t_tree *node);
void		free_flat_tree_list(t_flat_tree *flat_list);//todo doblicateed
void		free_flat_tree(t_flat_tree *flat_tree);
void		free_tree(t_tree *root);
	//redirection
int			parenths_redirections(t_tree *tree_node, t_token *token);
int			new_red(t_tree  *tree_node, t_type type, char *data);
int			is_red(t_token *token);
	//flat_tree
t_flat_tree	*flat_tree_last(t_flat_tree *flat_tree);
int			append_new_flat_tree_node(t_flat_tree **flat_tree_list, t_tree *tree_node);
t_flat_tree	*create_flat_tree(t_token *token);
int			skip_listed_tokens(t_token *token, int *i);
int			open_parenths_block(t_token *token, t_tree **tree_node, int *i);
int			close_parenths_block(t_tree **tree_node, int *i);

//tree
t_flat_tree	*create_flat_tree(t_token *tokens);
t_tree		build_the_tree(t_tree *tree, t_tree *flat_tree);
int			get_next_root(t_tree *flat_tree, int len, int side);
int			collect_reds_and_cmds(t_token *token, int *i, t_tree *tree_node, t_flat_tree *flat_tree_list);
int			collect_words(t_token *token, int i, t_tree *tree_node);
int			collect_redirections(t_token *token, int i, t_tree *tree_node);



//utils
int			get_list_size(t_node *list);

//tokenizer.c
int			tokenizer(t_token **tokenized_input, char *input);
int			create_token(t_token *token, char *data, int type);
int			count_splitted_input_nodes(t_node *splitted_input);
void		free_splitted_input(t_node *splitted_inpt);
void		free_tokens_list(t_token *tokens);


//p_commands
char		**append_command(char **old_list, char *new_cmd);

//redirection
void		append_redirection(t_red *redirections_list, char *data, t_type type);


int			update_last_executed_command(t_env **env_list,char *key, char *last_command);//idea this for the execution 
// idea If you remove this idea, I will cancel the team. Give me the money.

//lable expend
char		*join_arr_elements(char **arr);
void		update_quote(char str_i, int *quote);
void		skip_single_quote_area(char *str, int *i);
void		free_2d_arr(char **arr);
char		*extract_target_key(char *str);



//syntax_validate
int			validate_sytax(t_token *token);
int			validate_quotes(char *str);
int			validate_open_parenths(t_token *token);
int			validate_close_parenths(t_token *token);
void		put_operator_syntax_error_msg(t_type type);
void		put_redirections_syntax_error_msg(t_type type);
int			validate_single_parenth(t_token *tokens, int i, int *p);
int			is_opperator_syntax(t_type type);

//expand
int			expand_redir(t_red *reds, t_env *env);
int			expand_herdoc(char **str, t_env *env);
char		*delimiter_clear_dollar(char *str);
int			remove_non_printable_characters(char ***data);
int			generate_new_data_str(char **dst, char *str);
int			expand_herdoc_delimiter(t_red *reds, t_env *env);

void	print(char *s);//delete this


void update_env_exit_status(t_env **env_list, int status);
#endif