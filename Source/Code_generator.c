#include "Code_generator.h"

t_code	*new_code()
{
	t_code *c;

	c = (t_code *)malloc(sizeof(t_code));
	c->in = 0;
	c->s2 = 0;
	c->s1 = 0;
	return (c);
}

int		check_code(int c)
{
	static int *numbs = 0;
	int	i;

	if (numbs == 0)
	{
		numbs = (int *)malloc(2 * sizeof(int));
		numbs[0] = c;
		numbs[1] = 0;
	}
	else
	{
		i = -1;
		while (numbs[++i] != 0)
			if (numbs[i] == c)
				return (0);
		numbs = (int *)realloc(numbs, (i + 2) * sizeof(int));
		numbs[i] = c;
		numbs[i + 1] = 0;
	}
	return (1);
}

void	out_code(t_code *code, int tabs)
{
	int i;

	if (code->s1 != 0)
	{
		i = -1;
		while (++i < tabs)
			printf("   ");
		printf("%s\n", code->s1);
	}
	if (code->in != 0)
		if (code->s2 == 0)
			out_code(code->in, tabs);
		else
			out_code(code->in, tabs + 1);
	if (code->s2 != 0)
		printf("%s\n", code->s2);
}

void	out_code_file2(t_code *code, int tabs, int fd)
{
	int i;
	if (code->s1 != 0)
	{
		i = -1;
		while (++i < tabs)
			_write(fd, "   ", 3);
		_write(fd, code->s1, ft_strlen(code->s1));
		_write(fd, "\n", 1);
	}
	if (code->in != 0)
		if (code->s2 == 0)
			out_code_file2(code->in, tabs, fd);
		else
			out_code_file2(code->in, tabs + 1, fd);
	if (code->s2 != 0)
	{
		_write(fd, code->s2, ft_strlen(code->s2));
		_write(fd, "\n", 1);
	}
}

void	out_code_file(t_code *code, int tabs, int filename)
{
	int fd;

	fd = _open(filename, O_WRONLY | O_TRUNC);
	if (fd < 0)
		return;
	out_code_file2(code, tabs, fd);
}

t_code	*code_generator(t_leks_list *leks, char *filename)
{
	t_code	*code;
	t_code	*tmp;
	char	*stmp;

	stmp = (char *)malloc(START_BUFF);
	code = new_code();
	tmp = code;
	if (leks->numb == 401)
	{
		sprintf(stmp, ";%s\nCODE SEGMENT", leks->next->s);
		tmp->s1 = ft_strdup(stmp);
		tmp->s2 = ft_strdup("CODE ENDS");
	}
	else
	{
		sprintf(stmp, "PROCEDURE_%s PROC", leks->next->s);
		tmp->s1 = ft_strdup(stmp);
		sprintf(stmp, "PROCEDURE_%s ENDP", leks->next->s);
		tmp->s2 = ft_strdup(stmp);
	}
	tmp->in = new_code();
	tmp = tmp->in;
	leks = leks->next->next->next;
	if (leks->numb == 41)
		leks = leks->next->next;
	if (leks->numb == 403)
		while (leks->numb != 59)
		{
			if (check_code(leks->next->numb) == 1)
			{
				sprintf(stmp, "?%s:", leks->next->s);
				tmp->s1 = ft_strdup(stmp);
			}
			else
			{
				sprintf(stmp, "Code Generator: Error (line %d, column %d): Label '%s' already taken", leks->next->pos.line, leks->next->pos.column, leks->next->s);
				tmp->s1 = ft_strdup(stmp);
				return (code);
			}
			tmp->in = new_code();
			tmp = tmp->in;
			leks = leks->next->next;
		}
	return (code);
}