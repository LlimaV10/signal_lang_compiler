#ifndef __SYNTAX_H
# define __SYNTAX_H
# include "leks.h"

//Список для збереження дерева розбору
typedef struct	s_parser_tree
{
	char					*s;
	struct s_parser_tree	*in;
	struct s_parser_tree	*next;
}				t_parser_tree;

#endif