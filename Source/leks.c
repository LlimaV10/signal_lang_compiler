#include "leks.h"

// Створює новий, пустий елемент списку
t_leks_list		*ft_new_leks()
{
	t_leks_list		*l;

	l = (t_leks_list *)malloc(sizeof(t_leks_list));
	l->s = (char *)malloc(START_BUFF);
	l->next = 0;
	return (l);
}

// Додає елемент у початок списку
void			ft_add_leks(t_leks_list **start, t_leks_list *new, int len)
{
	new->s = (char *)realloc(new->s, len + 1);
	if (*start == 0)
		*start = new;
	else
	{
		new->next = *start;
		*start = new;
	}
}

int		is_lit(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return (1);
	return (0);
}

int		is_dig(char c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	return (0);
}

int		is_dm(char c, const char DM[COUNT_DM])
{
	int		i;

	i = -1;
	while (++i < COUNT_DM)
		if (c == DM[i])
			return (1);
	return (0);
}

int		is_ws(char c)
{
	if (c == 32 || c == 13 || c == 10 || c == 9 || c == 11 || c == 12)
		return (1);
	return (0);
}

// Записує у список лексичну помилку(не зарезервований символ) та пропускає лексему у якій була зроблена помилка
void	lekser_error(char c, t_position *curr, t_leks_list *leks, t_leks_list **start)
{
	sprintf(leks->s, "Lexer: Error (line %d, column %d): Illegal character '%c' detected", curr->line, curr->column, c);
	leks->numb = -1;
	ft_add_leks(start, leks, ft_strlen(leks->s));
}

// Записує у список лексичну помилку (Невідома лексема)
void	lekser_error2(t_leks_list *leks, t_leks_list **start)
{
	char	*tmp;
	char	*tmp2;

	tmp = (char *)malloc(START_BUFF);
	sprintf(tmp, "Lexer: Error (line %d, column %d): Undefined Identifier '%s'", leks->pos.line, leks->pos.column, leks->s);
	tmp2 = leks->s;
	leks->s = tmp;
	free(tmp2);
	leks->numb = -1;
	ft_add_leks(start, leks, ft_strlen(leks->s));
}

// Помилка (не закритий комментарій)
void	lekser_error3(t_leks_list *leks, t_leks_list **start)
{
	sprintf(leks->s, "Lexer: Error (line %d, column %d): not closed comment.", leks->pos.line, leks->pos.column);
	leks->numb = -1;
	ft_add_leks(start, leks, ft_strlen(leks->s));
}

// Змінює поточну позицію під час зчитування
void	next_curr(t_position	*curr, char c)
{
	if (c == '\n')
	{
		curr->line++;
		curr->column = 1;
	}
	else
		if (c == '\t')
			curr->column += 4;
		else
			curr->column++;
}

int		check_identifier(char *s, int numb, char ***ret)
{
	static char		**idt = 0;
	static int		i = 0;
	char			**tmp;
	int				j;

	if (ret != 0)
	{
		*ret = idt;
		return (0);
	}
	if (idt == 0)
	{
		i++;
		idt = (char **)malloc((i + 1) * sizeof(char *));
		idt[i - 1] = ft_strdup(s);
		idt[i] = 0;
		return (numb + i);
	}
	tmp = idt;
	j = 0;
	while (*tmp != 0)
	{
		j++;
		if (ft_strcmp(s, *tmp) == 0)
			return (numb + j);
		tmp++;
	}
	i++;
	idt = (char **)realloc(idt, (i + 1) * sizeof(char *));
	idt[i - 1] = ft_strdup(s);
	idt[i] = 0;
	return (numb + i);
}

int		check_number(char *s, int numb, char ***ret)
{
	static char		**idt = 0;
	static int		i = 0;
	char			**tmp;
	int				j;

	if (ret != 0)
	{
		*ret = idt;
		return (0);
	}
	if (idt == 0)
	{
		i++;
		idt = (char **)malloc((i + 1) * sizeof(char *));
		idt[i - 1] = ft_strdup(s);
		idt[i] = 0;
		return (numb + i);
	}
	tmp = idt;
	j = 0;
	while (*tmp != 0)
	{
		j++;
		if (ft_strcmp(s, *tmp) == 0)
			return (numb + j);
		tmp++;
	}
	i++;
	idt = (char **)realloc(idt, (i + 1) * sizeof(char *));
	idt[i - 1] = ft_strdup(s);
	idt[i] = 0;
	return (numb + i);
}

int		get_ident_code(char *s, const char *key_w[COUNT_KEYW])
{
	int		i;

	i = -1;
	while (++i < COUNT_KEYW)
		if (ft_strcmp(s, key_w[i]) == 0)
			return (401 + i);
	return (check_identifier(s, 1000, 0));
}

// Процедура, що повертає список лексем, їх кодів та позицію(line, column) 
t_leks_list	*lekser(char *filename)
{
	const char		*key_w[COUNT_KEYW] = { "PROGRAM", "PROCEDURE", "LABEL", "BEGIN", "END" };
	const char		DM[COUNT_DM] = { '(', ')', ';', '.', ',' };
	t_position		curr; // Поточна позиція
	t_leks_list		*start;
	t_leks_list		*now;
	int		symb_n;
	int		fd;
	int		rd;
	char	c;

	fd = _open(filename, O_RDONLY);
	rd = _read(fd, &c, 1);
	curr.column = 1;
	curr.line = 1;
	start = 0;
	while (rd == 1)
	{
		symb_n = 0;
		now = ft_new_leks();
		if (is_lit(c))
		{
			now->pos = curr;
			while ((is_lit(c) || is_dig(c)) && (rd == 1))
			{
				next_curr(&curr, c);
				(now->s)[symb_n++] = c;
				rd = _read(fd, &c, 1);
			}
			(now->s)[symb_n] = '\0';
			now->numb = get_ident_code(now->s, key_w);
			ft_add_leks(&start, now, symb_n);
		}
		else if (is_dig(c))
		{
			now->pos = curr;
			while ((is_dig(c)) && (rd == 1))
			{
				next_curr(&curr, c);
				(now->s)[symb_n++] = c;
				rd = _read(fd, &c, 1);
			}
			if (is_lit(c))
			{
				while (((!is_ws(c)) && (!is_dm(c, DM))) && (rd == 1))
				{
					next_curr(&curr, c);
					(now->s)[symb_n++] = c;
					rd = _read(fd, &c, 1);
				}
				now->s[symb_n] = '\0';
				lekser_error2(now, &start);
				continue;
			}
			(now->s)[symb_n] = '\0';
			now->numb = check_number(now->s, 500, 0);
			ft_add_leks(&start, now, symb_n);
		}
		else if (c == '(')
		{
			now->pos = curr;
			next_curr(&curr, c);
			rd = _read(fd, &c, 1);
			if (c == '*')
			{
				while ((c != ')') && (rd == 1))
				{
					do
					{
						next_curr(&curr, c);
						rd = _read(fd, &c, 1);
					} while ((c != '*') && (rd == 1));
					next_curr(&curr, c);
					rd = _read(fd, &c, 1);
				}
				if (rd != 1)
					lekser_error3(now, &start);
				else
				{
					free(now->s);
					free(now);
					next_curr(&curr, c);
					rd = _read(fd, &c, 1);
				}
			}
			else
			{
				now->numb = '(';
				now->s[0] = '(';
				now->s[1] = '\0';
				ft_add_leks(&start, now, 1);
			}
		}
		else if (is_dm(c, DM))
		{
			now->pos = curr;
			now->numb = c;
			now->s[0] = c;
			now->s[1] = '\0';
			ft_add_leks(&start, now, 1);
			next_curr(&curr, c);
			rd = _read(fd, &c, 1);
		}
		else if (is_ws(c))
			while ((is_ws(c)) && (rd == 1))
			{
				next_curr(&curr, c);
				rd = _read(fd, &c, 1);
			}
		else
		{
			lekser_error(c, &curr, now, &start);
			next_curr(&curr, c);
			rd = _read(fd, &c, 1);
		}
	}
	// EOF
	_close(fd);
	return (start);
}

// Вивід лексеми
void	output_leks(t_leks_list *start, char error)
{
	if (start->next != 0)
		output_leks(start->next, error);
	if (start->numb != -1)
	{
		if (!error)
			printf("%-4d%-4d%-5d %s\n", start->pos.line, start->pos.column, start->numb, start->s);
	}
	else
		printf("%s\n", start->s);
}

void	output_leks_file_2(t_leks_list *start, int fd, char *tmp, char error)
{
	if (start->next != 0)
		output_leks_file_2(start->next, fd, tmp, error);
	if (start->numb != -1)
		if (!error)
		{
			sprintf(tmp, "%-4d%-4d%-5d %s\n", start->pos.line, start->pos.column, start->numb, start->s);
			_write(fd, tmp, ft_strlen(tmp));
		}
	else
	{
		sprintf(tmp, "%s\n", start->s);
		_write(fd, tmp, ft_strlen(tmp));
	}
}

void	output_leks_file(t_leks_list *start, char error, const char *filename)
{
	int		fd;
	char	*tmp;

	tmp = (char *)malloc(100);
	fd = _open(filename, O_WRONLY | O_CREAT);
	if (fd < 0)
		return;
	output_leks_file_2(start, fd, tmp, error);
	free(tmp);
	_close(fd);
}

void	out_table(char **s, int start, int fd)
{
	char *tmp;

	if (fd > 0)
	{
		if (start < 1001)
			_write(fd, "Numbers:\n", 9);
		else
			_write(fd, "Identifiers:\n", 13);
		tmp = (char *)malloc(START_BUFF);
		while (*s != 0)
		{
			sprintf(tmp, "%-6d%s\n", start++, *s);
			_write(fd, tmp, ft_strlen(tmp));
			s++;
		}
		_write(fd, "\n", 1);
		free(tmp);
	}
	else
	{
		printf("\n\n");
		if (start < 1001)
			printf("Numbers:\n");
		else
			printf("Identifiers:\n");
		while (*s != 0)
			printf("%-6d%s\n", start++, *(s++));
	}
}

int		leks_error(t_leks_list *l)
{
	while (l != 0)
	{
		if (l->numb < 0)
			return (1);
		l = l->next;
	}
	return (0);
}

void	reverse_list(t_leks_list *leks, t_leks_list **newl)
{
	if (leks->next != 0)
		reverse_list(leks->next, newl);
	(*newl)->next = leks;
	*newl = (*newl)->next;
}

t_leks_list	*get_reversed_list(t_leks_list *leks)
{
	t_leks_list	*newl;
	t_leks_list *tmp;

	newl = ft_new_leks();
	tmp = newl;
	reverse_list(leks, &newl);
	newl->next = 0;
	newl = tmp->next;
	free(tmp->s);
	free(tmp);
	return (newl);
}