/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/26 19:40:18 by alelievr          #+#    #+#             */
/*   Updated: 2015/04/01 22:57:42 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include "libft.h"
# include <stdint.h>
# include <wchar.h>
# include <unistd.h>

# define FLAG "#0+-hljz123456789. "
# define S2(op) (op == 'o' || op == 'O')
# define SIGNED(op) (op == 'x' || op == 'X' || op == 'u' || op == 'U' || S2(op))

# define ZERO		0
# define PLUS		1
# define NORM		2
# define H			3
# define HH			4
# define SPACE		5
# define POINT		6
# define ALIGNE		7
# define NEG		8
# define C_POINT	9
# define F_ZERO		10
# define SHARP		11

extern int		g_conv;
extern int64_t	g_tmp;
extern int		g_wstr[0xF000];
extern int		g_retlen;

# define INT 0
# define UINT 1
# define LINT 2
# define LUINT 3
# define LLINT 4
# define LLUINT 5
# define INTMAX 6
# define UINTMAX 7

typedef struct	s_data
{
	uintmax_t		umax;
	intmax_t		max;
	uint64_t		u64;
	size_t			st;
	long			l;
	long long		ll;
	unsigned int	u;
	int				i;
}				t_data;

typedef struct	s_htype
{
	short			s;
	unsigned short	us;
	char			c;
	unsigned char	uc;
}				t_htype;

typedef struct	s_align
{
	char	op;
	char	*value;
	char	*buff;
	char	*opl;
	int		align;
	int		retadd;
	int		len;
}				t_align;

typedef struct	s_vsprintf
{
	int		len;
	int		len2;
	char	*tmp;
	int		index;
	char	*format;
	char	*buff;
	char	*expr;
}				t_vsprintf;

int				ft_putwchar_t(va_list *ap, char *buff, int i);
int				ft_putwstr_t(va_list *ap, char *buff, int i);
int				ft_pputstr(va_list *ap, char *buff, int i);
int				ft_pputchar(va_list *ap, char *buff, int i);
int				ft_putaddr(va_list *ap, char *buff, int i);
int				ft_pputchar(va_list *ap, char *buff, int i);
int				ft_getint(va_list *ap, char *buff, int i);
int				ft_float(va_list *ap, char *buff, int i);
void			ft_putwchart(wchar_t wchar, int *len, char *buff, int i);

int				ft_align(char *value, char *buff, char *opl, char op);
void			ft_align5(t_align *a);
void			ft_aligninit(t_align *a, char *value, char *opl, char *buff);
int				ft_zero(char *value, char *buff, char *opl, char op);
int				ft_sharp(char *value, char *buff, char *opl, char op);
int				ft_plus(char *value, char *buff, char *opl, char op);
int				ft_long(char *value, char *buff, char *opl, char op);
int				ft_short(char *value, char *buff, char *opl, char op);
int				ft_point(char *value, char *buff, char *opl, char op);
int				ft_space(char *value, char *buff, char *opl, char op);

int				ft_fprintf(char *expr, int fd, ...);
int				ft_sprintf(char *buff, char *expr, ...);
int				ft_printf(char *expr, ...);
int				ft_vsprintf(char *buff, char *expr, va_list *ap);
int				ft_vprintf(char *expr, va_list *ap);
int				ft_vf_printf(char *expr, va_list *ap);

char			*ft_organize(char *buffer, char *format);
int				ft_itoa64(char *buffer, int64_t n);
int				nbr_bits(unsigned int nbr);
int				ft_setint(char *value, t_data *d, char op, int uns);
int				ft_itoau64(char *buffer, uint64_t n);

#endif
