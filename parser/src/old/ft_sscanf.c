/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sscanf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/31 21:56:16 by vdaviot           #+#    #+#             */
/*   Updated: 2016/08/31 21:56:29 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

static	void	skip_space(char **format, char **str)
{
	(*format) += 2;
	while (ft_isspace(**str))
		(void)(*str)++;
}

static int		skip_string(char **format, char **str)
{
	while (**format != '\\' && **format != '%')
	{
		if (**format != **str)
			return (0);
		else
			(void)((long)(*str)++ + (long)++(*format));
	}
	return (1);
}

static int		convert_float(char **format, char **str, float *f)
{
	if (!(ft_isdigit(**str) || **str == '-'))
		return (0);
	*f = (float)ft_atod(*str);
	while (ft_isdigit(**str) || **str == '.' || **str == 'f' || **str == '-' || **str == '+')
		(void)(*str)++;
	(*format) += 2;
	return (1);
}

static int		convert_int(char **format, char **str, int *i)
{
	if (!(ft_isdigit(**str)))
		return (0);
	*i = ft_atoi(*str);
	while (ft_isdigit(**str) || **str == '-')
		(void)(*str)++;
	(*format) += 2;
	return (1);
}

static	int		convert_str(char **format, char **str, char *ptr)
{
	if (!(ft_isascii(**str) || **str == ' '))
		return (0);
	ft_strcpy(ptr, *str);
	while ((ft_isalpha(**str) || **str == '_' || **str == '.' || ft_isdigit(**str) || **str == '-'))
		(void)(*str)++;
	(*format) += 2;
	return (1);
}

int			ft_sscanf(char *format, char *str, ...)
{
	va_list	vargs;

	va_start(vargs, str);
	while (42)
	{
		if (*format == '\\' && format[1] == 's')
			skip_space(&format, &str);
		if (!*str)
			break ;
		if (*format == '%' && format[1] == 'f')
			if (!convert_float(&format, &str, va_arg(vargs, float *)))
				return (-1);
		if (*format == '%' && format[1] == 'd')
			if (!convert_int(&format, &str, va_arg(vargs, int *)))
				return (-1);
		if (*format == '%' && format[1] == 's')
			if (!convert_str(&format, &str, va_arg(vargs, char *)))
				return (-1);
		if (!skip_string(&format, &str))
			return (-1);
	}
	if (!*format)
		return (0);
	return (-1);
}
