/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobonill <mobonill@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:04:02 by mobonill          #+#    #+#             */
/*   Updated: 2024/07/10 11:16:20 by mobonill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int	ft_printf(const char *str, ...);
int	ft_ovpt(va_list args, char c);
int	ft_putchar_printf(char c);
int	ft_putstr_printf(char *str);
int	ft_putnbr_printf(int nb);
int	ft_putnbr_base_printf(unsigned long long nb, char *base,
		unsigned long long len);
int	ft_putaddress_printf(unsigned long long ptr);

#endif
