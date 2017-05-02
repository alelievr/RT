/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sscanf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/02 16:48:41 by pmartine          #+#    #+#             */
/*   Updated: 2017/05/02 17:47:05 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

int				convert_color(char **format, char **str, FTRGB *b)
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
