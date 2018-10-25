#include "syntax.h"

//Синтаксичний аналізатор

t_parser_tree	*new_branch()
{
	t_parser_tree	*t;

	t = (t_parser_tree *)malloc(sizeof(t_parser_tree));
	t->in = 0;
	t->next = 0;
	t->s = 0;
//	t->reserve_error = 0;
	return (t);
}

void	insert_equivalent(char **s, char *or)
{
	int		lens;
	int		lenor;
	char	*tmp;
	char	*tmp2;

	if (*(tmp = ft_strchr(*s, 39)) == 0)
		return;
	lens = ft_strlen(*s);
	lenor = ft_strlen(or) + 8;
	*s = (char *)realloc(*s, lens + lenor + 1);
	tmp2 = *s + lens;
	while (tmp2 != tmp)
	{
		tmp2[lenor] = *tmp2;
		tmp2--;
	}
	tmp2[lenor] = *tmp;
	*(tmp++) = 39;
	while (*or != '\0')
		*(tmp++) = *(or++);
	if (*(or -1) != '>')
	{
		*(tmp++) = 39;
		*(tmp++) = ' ';
		*(tmp++) = ' ';
		*(tmp++) = 'o';
		*(tmp++) = 'r';
		*(tmp++) = ' ';
		*(tmp++) = ' ';
	}
	else
	{
		*(tmp++) = 39;
		*(tmp++) = ' ';
		*(tmp++) = 't';
		*(tmp++) = 'h';
		*(tmp++) = 'e';
		*(tmp++) = 'n';
		*(tmp++) = ' ';
	}
}

int		Parser_error(t_position pos, const char *exp, const char *found, t_parser_tree **tree, t_parser_tree *tmp, char *reserve)//t_position pos, char *exp, char *found, t_parser_tree **tree, t_parser_tree *tmp)
{
	(*tree)->s = (char *)malloc(67 + ft_strlen(exp) + ft_strlen(found));
	if (pos.line != 0)
		sprintf((*tree)->s, "Parser: Error (line %d, column %d): '%s' expected but '%s' found", pos.line, pos.column, exp, found);
	else
		sprintf((*tree)->s, "Parser: Error: '%s' expected but '%s' found", exp, found);
	if (reserve != 0)
		insert_equivalent(&((*tree)->s), reserve);
	if (tmp != 0)
		*tree = tmp;
	return (0);
}

int 	syntax_analizer(t_leks_list **leks, unsigned const short k[31][3],
	const char names[31][23], t_parser_tree **tree, unsigned short id, char **reserve_error)
{
	t_parser_tree	*tmp;
	t_parser_tree	*tmp2;

	if (id >= 31)
	{
		(*tree) = new_branch();
		if (id == 32)
		{
			if (((*leks)->numb >= 501) && ((*leks)->numb <= 1000))
			{
				(*tree)->s = (char *)malloc(ft_strlen((*leks)->s) + 7);
				sprintf((*tree)->s, "%d %s", (*leks)->numb, (*leks)->s);
				*leks = (*leks)->next;
			}
			else
				return (Parser_error((*leks)->pos, "<unsigned-integer>", (*leks)->s, tree, 0, *reserve_error));
		}
		else if (id == 31)
		{
			if ((*leks)->numb >= 1001)
			{
				(*tree)->s = (char *)malloc(ft_strlen((*leks)->s) + 7);
				sprintf((*tree)->s, "%d %s", (*leks)->numb, (*leks)->s);
				*leks = (*leks)->next;
			}
			else
				return (Parser_error((*leks)->pos, "<identifier>", (*leks)->s, tree, 0, *reserve_error));
		}
		else
			(*tree)->s = ft_strdup("<empty>");
		return (1);
	}
	if (*tree == 0)
		*tree = new_branch();
	tmp = *tree;
	do {
		if ((*leks) == 0)
		{
			int		ret;
			t_leks_list *fake;

			fake = ft_new_leks();
			fake->pos.line = 0;
			fake->numb = -1;
			fake->s = ft_strdup("EOF");
			ret = syntax_analizer(&fake, k, names, tree, id, reserve_error);
			free(fake->s);
			free(fake);
			*tree = tmp;
			return (ret);
		}
		if (k[id][0] <= 33)
		{
			int ret;
			(*tree)->s = ft_strdup(names[id]);
			if (k[id][0] != 33)
			{
				if ((ret = syntax_analizer(leks, k, names, &((*tree)->in), k[id][0], reserve_error)) == 0)
				{
					*tree = tmp;
					return (0);
				}
			}
			else
				*reserve_error = names[id];
		}
		else if (k[id][0] == (*leks)->numb)
		{
			*reserve_error = 0;
			(*tree)->s = (char *)malloc(ft_strlen(names[id]) + 7);
			sprintf((*tree)->s, "%d %s", (*leks)->numb, names[id]);
			*leks = (*leks)->next;
		}
		else if (k[id][2] != 0)
		{
			*reserve_error = names[id];
			return(syntax_analizer(leks, k, names, tree, k[id][2], reserve_error));
		}
		else
			return (Parser_error((*leks)->pos, names[id], (*leks)->s, tree, tmp, *reserve_error));
		(*tree)->next = new_branch();
		*tree = (*tree)->next;
	} while (k[id++][1] == 0);
	tmp2 = tmp;
	while (tmp2->next != *tree)
		tmp2 = tmp2->next;
	free(tmp2->next);
	tmp2->next = 0;
	*tree = tmp;
	return (1);
}

void	free_tree(t_parser_tree	*tree)
{
	if (tree != 0)
	{
		free_tree(tree->in);
		free_tree(tree->next);
		free(tree);
	}
}

char	*get_syntax_error(t_parser_tree *t)
{
	char	*s;

	while (t != 0)
	{
		if (((t->s)[0] == 'P') && ((t->s)[1] == 'a'))
			return (t->s);
		if (t->in != 0)
			if ((s = get_syntax_error(t->in)) != 0)
				return (s);
		t = t->next;
	}
	return (0);
}

t_parser_tree	*get_tree(t_leks_list	*leks)
{
	t_parser_tree	*tree;
	int				ret;
	t_parser_tree	*tmp;
	char			*reserve_error;

	unsigned const short	knyt[31][3] =
		// Code AT AF
	{ { 0, 0, 0 },
	{ 2, 1, 0 }, //		<signal-program>
	{ 401, 0, 7 }, //	<program>
	{ 30, 0, 0 },
	{ 59, 0, 0 },
	{ 13, 0, 0 },
	{ 46, 1, 0 },
	{ 402, 0, 0 }, //	<program>
	{ 30, 0, 0 },
	{ 25, 0, 0 },
	{ 59, 0, 0 },
	{ 13, 0, 0 },
	{ 59, 1, 0 },
	{ 17, 0, 0 }, //	<block>
	{ 404, 0, 0 },
	{ 29, 0, 0 },
	{ 405, 1, 0 },
	{ 18, 1, 0 }, //	<declarations>
	{ 403, 0, 33 }, //	<label_declarations>
	{ 32, 0, 0 },
	{ 22, 0, 0 },
	{ 59, 1, 0 },
	{ 44, 0, 33 }, //	<labels-list>
	{ 32, 0, 0 },
	{ 22, 1, 0 },
	{ 40, 0, 33 }, //	<parameters-list>
	{ 28, 0, 0 },
	{ 41, 1, 0 },
	{ 33, 1, 0 }, //	<declarations-list>
	{ 33, 1, 0 }, //	<statements-list>
	{ 31, 1, 0 }  //	<procedure-identifier>
	};
	const char	names[31][23] =
	{
		"<signal-program>",
		"<program>",
		"PROGRAM",
		"<procedure-identifier>",
		";",
		"<block>",
		".",
		"PROCEDURE",
		"<procedure-identifier>",
		"<parameters-list>",
		";",
		"<block>",
		";",
		"<declarations>",
		"BEGIN",
		"<statements-list>",
		"END",
		"<label-declarations>",
		"LABEL",
		"<unsigned-integer>",
		"<labels-list>",
		";",
		",",
		"<unsigned-integer>",
		"<labels-list>",
		"(",
		"<declarations-list>",
		")",
		"<empty>",
		"<empty>",
		"<identifier>"
	};
	tree = new_branch();
	tree->s = ft_strdup(names[0]);
	reserve_error = 0;
	ret = syntax_analizer(&leks, knyt, names, &(tree->in), 1, &reserve_error);
	if ((leks != 0) && (ret == 1))
	{
		tmp = tree;
		while (tmp->next != 0)
			tmp = tmp->next;
		tmp->next = new_branch();
		tmp->next->s = (char *)malloc(68 + ft_strlen(leks->s));
		sprintf(tmp->next->s, "Parser: Error (line %d, column %d): EOF expected but '%s' found", leks->pos.line, leks->pos.column, leks->s);
		ret = 0;
	}
	if (ret == 0)
	{
		tmp = new_branch();
		tmp->s = ft_strdup(get_syntax_error(tree));
		free_tree(tree);
		return (tmp);
	}
	return (tree);

}

void	print_tree(t_parser_tree *t, int tabs)
{
	int		i;

	while (t != 0)
	{
		i = -1;
		if (!(((t->s)[0] == 'P') && ((t->s)[1] == 'a')))
			while (++i < tabs)
				printf("|....");
		printf("%s\n", t->s);
		if (t->in != 0)
			print_tree(t->in, tabs + 1);
		t = t->next;
	}
}

void	print_tree_file2(t_parser_tree *t, int tabs, int fd)
{
	int		i;

	while (t != 0)
	{
		i = -1;
		if (!(((t->s)[0] == 'P') && ((t->s)[1] == 'a')))
			while (++i < tabs)
				_write(fd, "|....", 5);
		_write(fd, t->s, ft_strlen(t->s));
		_write(fd, "\n", 1);
		if (t->in != 0)
			print_tree_file2(t->in, tabs + 1, fd);
		t = t->next;
	}
}

void	print_tree_file(t_parser_tree *t, int tabs, char *filename)
{
	int		fd;

	fd = _open(filename, O_WRONLY);
	print_tree_file2(t, tabs, fd);
	_close(fd);
}