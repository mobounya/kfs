#include <string.h>

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

char			*itoa(uint64_t nbr)
{
	static char str[24];
	int			i;

	if (nbr == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	i = 0;
	while (nbr > 0)
	{
		str[i++] = '0' + (nbr % 10);
		nbr = nbr / 10;
	}
	str[i] = '\0';
	strrev(str);
	return (str);
}
