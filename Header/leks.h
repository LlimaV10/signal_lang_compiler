#ifndef __LEKS_H
# define __LEKS_H
# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h> // prinf
# include <stdlib.h> //malloc
# include <fcntl.h> // O_RDONLY
# include <io.h> // __open __read __close

// Теоретична кількість початкових байтів на ідентифікатор (потім урізається)
# define		START_BUFF	1000

# define		COUNT_KEYW	5
# define		COUNT_DM	5

// Для збереження позиції
typedef struct	s_position
{
	int		line;
	int		column;
}				t_position;

// Список у якому буде зберігатися кожна лексема
typedef struct	s_leks_list
{
	t_position			pos; // позиція початку лексеми
	int					numb; // Код лексеми
	char				*s; // Сама лексема
	struct s_leks_list	*next; // Попередьня лексема
}				t_leks_list;

#endif