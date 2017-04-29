/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/19 18:41:31 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/28 20:35:21 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"
#include <stdio.h>
#include <unistd.h>

#define DO(x) buff += ft_concat(buff, x, padd, align, flags)

static int	printf_wrapper(const char *format, va_list *args, char *buff)
{
	int			padd;
	int			align;
	int			flags;
	int			type;
	const char	*b = buff;

	while (*format)
	{
		if (*format == '%')
		{
			type = get_arg_flags(&format, &padd, &align, &flags, args);
			GET_NEXT_ARG(args, type);
		}
		else
			*buff++ = *format++;
	}
	*buff++ = 0;
	return (buff - b);
}

int			ft_printf(const char *format, ...)
{
	char	buff[0xF000];
	int		ret;
	va_list	args;

	va_start(args, format);
	ret = printf_wrapper(format, &args, buff);
	write(1, buff, ret);
	va_end(args);
	return (ret);
}

int			ft_sprintf(char *buff, const char *format, ...)
{
	int		ret;
	va_list	args;

	va_start(args, format);
	ret = printf_wrapper(format, &args, buff);
	va_end(args);
	return (ret);
}

int			ft_dprintf(int fd, const char *format, ...)
{
	char	buff[0xF000];
	int		ret;
	va_list	args;

	va_start(args, format);
	ret = printf_wrapper(format, &args, buff);
	write(fd, buff, ret);
	va_end(args);
	return (ret);
}

int			ft_vprintf(const char *format, va_list *ap)
{
	char	buff[0xF000];
	int		ret;

	ret = printf_wrapper(format, ap, buff);
	write(1, buff, ret);
	return (ret);
}
