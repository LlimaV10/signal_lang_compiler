#ifndef __LEKS_H
# define __LEKS_H
# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h> // prinf
# include <stdlib.h> //malloc
# include <fcntl.h> // O_RDONLY
# include <io.h> // __open __read __close

// ���������� ������� ���������� ����� �� ������������� (���� ��������)
# define		START_BUFF	1000

# define		COUNT_KEYW	5
# define		COUNT_DM	5

// ��� ���������� �������
typedef struct	s_position
{
	int		line;
	int		column;
}				t_position;

// ������ � ����� ���� ���������� ����� �������
typedef struct	s_leks_list
{
	t_position			pos; // ������� ������� �������
	int					numb; // ��� �������
	char				*s; // ���� �������
	struct s_leks_list	*next; // ���������� �������
}				t_leks_list;

#endif