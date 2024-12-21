/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:40:30 by mobonill          #+#    #+#             */
/*   Updated: 2024/12/20 14:23:16 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_fovpt(int fd, va_list args, const char c)
{
	int	count;

	count = 0;
	if (c == '%')
		count += ft_putchar_fprintf(fd, '%');
	else if (c == 'c')
		count += ft_putchar_fprintf(fd, va_arg(args, int));
	else if (c == 's')
		count += ft_putstr_fprintf(fd, va_arg(args, char *));
	else if (c == 'd')
		count += ft_putnbr_fprintf(fd, va_arg(args, int));
	else if (c == 'i')
		count += ft_putnbr_fprintf(fd, va_arg(args, int));
	else if (c == 'u')
		count += ft_putnbr_base_fprintf(fd, va_arg(args, unsigned int),
				"0123456789", 10);
	else if (c == 'x')
		count += ft_putnbr_base_fprintf(fd, va_arg(args, unsigned int),
				"0123456789abcdef", 16);
	else if (c == 'X')
		count += ft_putnbr_base_fprintf(fd, va_arg(args, unsigned int),
				"0123456789ABCDEF", 16);
	else if (c == 'p')
		count += ft_putaddress_fprintf(fd, va_arg(args, unsigned long long));
	return (count);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	va_list		args;
	int			i;
	static int	count;

	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			count += ft_fovpt(fd, args, str[i]);
		}
		else
		{
			ft_putchar_fprintf(fd, str[i]);
			count++;
		}
		i++;
	}
	va_end(args);
	return (count);
}
