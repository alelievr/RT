/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sscanf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <vdaviot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/31 21:56:16 by vdaviot           #+#    #+#             */
/*   Updated: 2017/04/29 13:00:01 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#define FTN float *r, float *g, float

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
	if (!(ft_isdigit(**str) || **str == '-' || **str == '.'))
		return (0);
	else
		*f = (float)ft_atof(*str);
	while (ft_isdigit(**str) || **str == '.'
			|| **str == 'f' || **str == '-' || **str == '+')
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

static	int		convert_word(char **format, char **str, char *ptr, int buffsize)
{
	int				len;
	const char		*begin_word = *str;

	len = 0;
	while (**str && !ft_isspace(**str)
			&& **str != ',' && **str != ':' && **str != '|')
		_((*str)++, len++);
	ft_strlcpy(ptr, begin_word, (len < buffsize + 1) ? len + 1 : buffsize);
	(*format) += 2;
	return (1);
}

static	int		convert_str(char **format, char **str, char *ptr, int buffsize)
{
	const char		*begin_str = *str;
	int				i;

	i = 0;
	while (**str && i < buffsize)
		_((*str)++, i++);
	ft_strlcpy(ptr, begin_str, buffsize);
	(*format) += 2;
	return (1);
}

void			rgbcolor(int color, float *r, float *g, float *b)
{
	*r = color >> 16 & 255;
	*g = color >> 8 & 255;
	*b = color & 255;
}

int				hexa(char *str)
{
	int	i;

	i = 1;
	if (str[i++] == 'x')
		while (ft_isdigit(str[i]) || (str[i] > 64 && str[i] < 71))
			i++;
	if (i == 8)
		return (1);
	return (0);
}

static int		convert_color(char **format, char **str, FTN *b)
{
	if (**str == '0' && hexa(*str))
	{
		(void)(*str)++;
		(void)(*str)++;
		rgbcolor(ft_ndeconvert(*str, 16, 6), r, g, b);
	}
	(*format) += 2;
	return (1);
}

int				sscanf_return(int value, va_list *vargs)
{
	va_end(*vargs);
	return (value);
}

int				ft_sscanf(char *format, char *str, ...)
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
				return (sscanf_return(-1, &vargs));
		if (*format == '%' && format[1] == 'd')
			if (!convert_int(&format, &str, va_arg(vargs, int *)))
				return (sscanf_return(-1, &vargs));
		if (*format == '%' && format[1] == 'w')
			if (!convert_word(&format, &str, va_arg(vargs, char *),
						va_arg(vargs, int)))
				return (sscanf_return(-1, &vargs));
		if (*format == '%' && format[1] == 's')
			if (!convert_str(&format, &str, va_arg(vargs, char *),
						va_arg(vargs, int)))
				return (sscanf_return(-1, &vargs));
		if (*format == '%' && format[1] == 'z')
			if (!convert_color(&format, &str, va_arg(vargs, float *),
						va_arg(vargs, float *), va_arg(vargs, float *)))
				return (sscanf_return(-1, &vargs));
		if (!skip_string(&format, &str))
			return (sscanf_return(-1, &vargs));
	}
	if (!*format)
		return (sscanf_return(0, &vargs));
	return (sscanf_return(-1, &vargs));
}
