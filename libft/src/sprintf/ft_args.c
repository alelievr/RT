/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/19 19:41:37 by alelievr          #+#    #+#             */
/*   Updated: 2017/01/12 23:13:36 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"
#include <stdarg.h>

#define O(x, y, z) ((**fmt == x) ? (y) : (z))
#define FLAG_HAS(flag, v) (flag & (v))
#define H_FLAG(flag) ((FLAG_HAS(flag, F_16BIT)) ? F_08BIT : F_16BIT)
#define L_FLAG(flag) ((FLAG_HAS(flag, F_48BIT)) ? F_64BIT : F_48BIT)
#define	CHAR_FLAG_TO_VAL(flag) (O('0', F_ZERO, O('-', F_NEGATIVE, O('+', F_PLUS, O(' ', F_SPACE, O('#', F_PREFIX, O('\'', F_SEPARATOR, O('h', H_FLAG(flag), O('l', L_FLAG(flag), 0)))))))))

#define SIZEOF_TYPE(t) (P(T_CHAR, sizeof(char), P(T_INTEGER, sizeof(int), P(T_LONG, sizeof(long), P(T_POINTER, sizeof(void *), sizeof(char *))))))

static int	g_additional_flags[127] = {0};

static void	init_additional_flags(void) __attribute__((constructor));
static void	init_additional_flags(void)
{
	g_additional_flags['c'] = 0;
	g_additional_flags['x'] = F_HEXA;
	g_additional_flags['X'] = F_HEXA | F_UPPERCASE;
	g_additional_flags['o'] = F_OCTAL;
	g_additional_flags['O'] = F_OCTAL | F_UPPERCASE;
	g_additional_flags['i'] = F_DECIMAL | F_SIGNED;
	g_additional_flags['d'] = F_DECIMAL | F_SIGNED;
	g_additional_flags['D'] = F_DECIMAL | F_SIGNED | F_64BIT;
	g_additional_flags['U'] = F_DECIMAL | F_UPPERCASE;
	g_additional_flags['e'] = F_EXPONENT;
	g_additional_flags['E'] = F_EXPONENT | F_UPPERCASE;
	g_additional_flags['p'] = F_PREFIX | F_HEXA | F_LOWERCASE | F_64BIT;
}

#include <stdio.h>
int			get_arg_flags(const char **fmt, int *padd, int *align, int *flags, va_list *ap)
{
	INIT(int, ret, 0);
	INIT(int, *r, align);
	INIT(char, *tc, NULL);
	DBI(DBA(r, align, *flags, F_32BIT), DBA(*padd, NOPADD, *align, NOALIGN));
	while (ft_strchr(ALLOWED_FLAGS, **fmt))
	{
		if (ft_isdigit(**fmt) && **fmt != '0')
			*r = ft_atoi(*fmt);
		if (**fmt == '.')
			r = padd;
		if (**fmt == '*')
			*r = get_next_arg(ap, T_INTEGER);
		*flags |= CHAR_FLAG_TO_VAL(*flags);
		(*fmt)++;
	}
	tc = (char[]){**fmt, 0};
	ret = CHAR_TO_TYPE(*fmt);
	if (ret == -1)
		ft_printf("use of undefined flag: %c\n", **fmt);
	*flags |= g_additional_flags[(int)*(*fmt)++];
	if (FLAG_HAS(*flags, F_NEGATIVE) && *align != NOALIGN)
		*align = -*align;
	return ret;
}

unsigned long long int	get_next_arg(va_list *ap, int type)
{
	if (SIZEOF_TYPE(type) > 4)
		return (unsigned long int)(va_arg(*ap, long));
	else
		return (unsigned long int)(va_arg(*ap, int));
}
