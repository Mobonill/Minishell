/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:35:55 by mobonill          #+#    #+#             */
/*   Updated: 2024/05/01 20:04:28 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_size(long int n)
{
	int		i;

	i = 1;
	if (n < 0)
	{
		n *= -1;
		i++;
	}
	while (n >= 10)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int			i;
	long int	nb;
	char		*str;

	nb = n;
	i = ft_size(nb);
	str = ft_calloc(sizeof(char), i + 1);
	if (!str)
		return (NULL);
	if (nb < 0)
		nb = -nb;
	while (i > 0)
	{
		i--;
		str[i] = nb % 10 + '0';
		nb = nb / 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
// int	main(void)
// {
// 	int	n = 487;
// 	char *str;
// 	str = ft_itoa(n);
// 	printf("%s", str);
// }