#ifndef __CODE_GENERATOR_H
# define __CODE_GENERATOR_H

# include "syntax.h"
# include <sys/stat.h>

typedef struct	s_code
{
	char			*s1;
	char			*s2;
	struct s_code	*in;
}				t_code;

#endif