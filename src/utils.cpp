//
// Created by Yellowjacket Manfryd on 1/29/22.
//

#include "../inc/utils.hpp"

char	*ft_memset(char *str, int sym, unsigned int size)
{
    unsigned int	i;

    i = 0;
    while (i < size)
    {
        str[i] = sym;
        i++;
    }
    return (str);
}

size_t	ft_strlen(const char *s)
{
    size_t	a;

    a = 0;
    if (!s)
        return (0);
    while (s[a])
        a++;
    return (a);
}

static int	size(int nb)
{
    int		s;
    long	n;

    n = nb;
    if (!n)
        return (1);
    s = 0;
    if (nb < 0)
    {
        s++;
        n = -n;
    }
    while (n)
    {
        s++;
        n = n / 10;
    }
    return (s);
}

char	*ft_itoa(int n)
{
    char	*a;
    int		i;
    long	nb;

    i = size(n);
    nb = n;
    a = (char *)malloc(sizeof(char) * (i + 1));
    if (!a)
        return (NULL);
    a[i--] = '\0';
    if (nb == 0)
        a[i] = '0';
    if (nb < 0)
    {
        a[0] = '-';
        nb = -nb;
    }
    while (nb > 0)
    {
        a[i--] = '0' + (nb % 10);
        nb = nb / 10;
    }
    return (a);
}
