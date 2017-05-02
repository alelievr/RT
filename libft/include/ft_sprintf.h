/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 20:28:54 by alelievr          #+#    #+#             */
/*   Updated: 2017/05/02 19:57:22 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SPRINTF_H
# define FT_SPRINTF_H

# include <string.h>
# include <limits.h>
# include <stdbool.h>
# include <wchar.h>
# include "libft.h"
# undef F_ZERO
# define NOPADD			(int)(INT_MIN)
# define NOALIGN		(int)(INT_MIN)
# define MAXALIGN		1024
# define MAXPADD		1024
# define ALLOWED_FLAGS	"0123456789-+.%*' #lhjz"
# define F_08BIT		1 << 0
# define F_16BIT		1 << 1
# define F_32BIT		1 << 2
# define F_48BIT		1 << 3
# define F_64BIT		1 << 4
# define F_MAXBIT		1 << 4
# define F_SIGNED		1 << 5
# define F_HEXA			1 << 6
# define F_OCTAL		1 << 7
# define F_DECIMAL		1 << 8
# define F_BINARY		1 << 9
# define F_PREFIX		1 << 12
# define F_PLUS			1 << 13
# define F_SPACE		1 << 14
# define F_SEPARATOR	1 << 15
# define F_ZERO			1 << 16
# define F_UPPERCASE	1 << 17
# define F_LOWERCASE	1 << 18
# define F_NEGATIVE		1 << 20
# define F_POSITIVE		1 << 21
# define F_EXPONENT		1 << 22
# define F_NOERROR		1 << 31
# define GUBN2__(x) ((x & F_OCTAL) ? (8) : ((x & F_BINARY) ? (2) : (10)))
# define GET_UNSIGNED_BASE_NUMBER(x) ((x & F_HEXA) ? (16) : (GUBN2__(x)))
# define GET_SIGNED_BASE_NUMBER(x) ((x & F_DECIMAL) ? 10 : 10)
# define F4(b, v, p, f) default: ft_write_val_padd_ulli)(b, v, p, f)
# define FTWP3(b,v,p,f)unsigned long long int:ft_write_val_padd_ulli,F4(b,v,p,f)
# define FTWP2(b, v, p, f) wchar_t *: ft_write_val_padd_wchars, FTWP3(b, v, p,f)
# define FTWVP1__(b, v, p, f) wchar_t: ft_write_val_padd_wchar, FTWP2(b, v, p,f)
# define FT_WRITE_VAL_PADD(b, v, p, f) _Generic((v), FTWVP1__(b, v, p, f)
# define ARG_II int align, int
# define TYPEIFY(type, data)
# define T(x, y, z) ((ft_strstr(x, tc) != NULL) ? (y) : (z))
# define P(v, x, y) ((type == v) ? (x) : (y))
# define CHAR_TO_TRUE_TYPE_CAST(x) (P(T_CHAR, DO((char)(x)), P(T_INTEGER, DO((unsigned long long int)(x)), P(T_POINTER, DO((unsigned long long int)(x)), P(T_WSTRING, DO((wchar_t *)(unsigned long long int)(x)), P(T_LONG, DO((unsigned long long int)(x)), P(T_WCHAR, DO((wchar_t)(x)), DO((char *)(x)))))))))
# define CHAR_TO_TYPE(tc) (T("c", T_CHAR, T("diouxX", T_INTEGER, T("DOU", T_LONG, T("p", T_POINTER, T("s", T_STRING, T("S", T_WSTRING, T("C", T_WCHAR, -1))))))))
# define GET_NEXT_ARG(ap, type) CHAR_TO_TRUE_TYPE_CAST(get_next_arg(ap, type))

enum			e_type
{
	T_CHAR,
	T_SHORT,
	T_INTEGER,
	T_LONG,
	T_LONGLONG,
	T_STRING,
	T_WCHAR,
	T_WSTRING,
	T_POINTER,
};

typedef struct	s_type
{
	int		type;
	size_t	size;
}				t_type;

/*
**	Write val padd functions:
*/
size_t			ft_write_val_padd_ulli(char *tmpbuff,
		unsigned long long int val, int padd, const int flag);
size_t			ft_write_val_padd_wchar(char *tmpbuff, wchar_t val,
		int padd, const int flag);
size_t			ft_write_val_padd_wchars(char *tmpbuff, wchar_t *val,
		int padd, const int flag);
size_t			ft_add_integer_prefix(char *buff, const int flag);
char			ft_get_base_char(int i, const int flag);
size_t			ft_intlen_base(long long int val, const int flag);
size_t			ft_uintlen_base(unsigned long long int val, const int flag);

/*
**	Concat funcions:
*/
size_t			ft_concat_align(char *buff, char *tmpbuff, size_t len,
		int align, int flag);
size_t			ft_concat(char *buff, char c, int padd, int align,
		int flag) __attribute__((overloadable));
size_t			ft_concat(char *buff, unsigned long long int val, int padd,
		int align, int flag) __attribute__((overloadable));
size_t			ft_concat(char *buff, char *str, int padd, int align,
		int flag) __attribute__((overloadable));
size_t			ft_concat(char *buff, wchar_t wch, int padd, int align,
		int flag) __attribute__((overloadable));
size_t			ft_concat(char *buff, wchar_t *wchrs, int padd, int align,
		int flag) __attribute__((overloadable));
size_t			ft_concat(char *buff, double d, int padd, int align,
		int flag) __attribute__((overloadable));

/*
**	Flag parsing and printf implementations:
*/
int				ft_printf(const char *format, ...);
int				ft_sprintf(char *buff, const char *format, ...);
int				ft_dprintf(int fd, const char *format, ...);

int				get_arg_flags(const char **fmt, int *padd, int *align, int *flags, va_list *ap);
unsigned long long int	get_next_arg(va_list *ap, int type);

#endif
