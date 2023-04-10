#include <cstring.h>

static	void	strrev(char *str)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = strlen(str) - 1;
	while (i < j)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
		j--;
		i++;
	}
}

char			*itoa(int n)
{
	static char str[24];
	int			i;
	unsigned	nbr;

	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	nbr = (n < 0) ? -n : n;
	i = 0;
	while (nbr > 0)
	{
		str[i++] = '0' + (nbr % 10);
		nbr = nbr / 10;
	}
	if (n < 0)
		str[i++] = '-';
	str[i] = '\0';
	strrev(str);
	return (str);
}

