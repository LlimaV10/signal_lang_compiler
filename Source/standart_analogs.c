#include "syntax.h"

// Аналог strlen()
unsigned int	ft_strlen(const char *s)
{
	unsigned int	i;

	if (*s == '\0')
		return (0);
	i = 1;
	while (*(++s) != '\0')
		i++;
	return (i);
}

// Аналог strdup
char	*ft_strdup(const char *s)
{
	char	*sd;
	char	*tmp;

	sd = (char *)malloc(ft_strlen(s) + 1);
	tmp = sd;
	while (*s != '\0')
		*(sd++) = *(s++);
	*sd = '\0';
	return (tmp);
}

// Аналог strcmp
unsigned int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 == *s2) && (*s1 != '\0') && (*s2 != '\0'))
	{
		s1++;
		s2++;
	}
	return ((unsigned int)*s1 - (unsigned int)*s2);
}

// Аналог strchr
char	*ft_strchr(const char *s, char c)
{
	while ((*s != '\0') && (*s != c))
		s++;
	return (s);
}