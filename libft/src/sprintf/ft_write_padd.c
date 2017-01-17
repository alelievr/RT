/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_padd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 21:13:00 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/21 12:59:24 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"
#define FWU(b, v, p, f)return ft_write_unsigned(b,(unsigned long long)(v),p,f)
#define FWS(b, v, p, f) return ft_write_signed(b, (long long int)(v), p, f)
#define IS_SIGNED(flag) (flag & F_SIGNED && flag & F_DECIMAL)
#define CHOOSE_SIGNED(x,y,p,f) {if(IS_SIGNED(f))FWS(x,y,p,f);else FWU(x,y,p,f);}

static char		*ft_write_padding_prefix_integer(char **buff, size_t *len,
		int padd, const int flag)
{
	INIT(size_t, tmp, 0);
	INIT(char *, pa, NULL);
	pa = NULL;
	if (flag & F_PREFIX && (flag & F_OCTAL || flag & F_HEXA))
	{
		tmp = ft_add_integer_prefix(*buff, flag);
		*buff += tmp;
	}
	if (padd != NOPADD && *len < (size_t)padd)
	{
		ft_memset(*buff, (flag & F_ZERO) ? '0' : ' ', padd - *len);
		pa = *buff;
		*buff += padd - 1;
		*len += padd - *len;
	}
	else
		*buff += *len - 1;
	*len += tmp;
	if ((flag & F_PLUS || flag & F_SPACE) && flag & F_POSITIVE)
	{
		if (pa == NULL)
			(void)((int)(*len)++ | (int)(*buff)++ | (int)(pa = (*buff) - 2));
		(*pa) = (flag & F_PLUS) ? '+' : ' ';
	}
	return (pa);
}

static size_t	ft_write_signed(char *buff, long long int val, int padd,
		const int flag)
{
	size_t				len;
	unsigned long long	n;
	const long long int	diviser = GET_SIGNED_BASE_NUMBER(flag);
	char				*padded_addr;

	len = ft_intlen_base(val, flag);
	padded_addr = ft_write_padding_prefix_integer(&buff, &len, padd,
			flag | ((val < 0) ? F_NEGATIVE : F_POSITIVE));
	n = (val < 0) ? (unsigned long long)-val : (unsigned long long)val;
	buff[1] = 0;
	*buff-- = ft_get_base_char((n % diviser), flag);
	if (val < 0)
		*buff = '-';
	while (n /= diviser)
		if (val < 0)
			*(short *)(unsigned long)--buff = (short)('-' |
					(ft_get_base_char(n % diviser, flag) << 8));
		else
			*buff-- = ft_get_base_char(n % diviser, flag);
	buff += (val < 0) ? 0 : 1;
	if (padd != NOPADD && padd > 0 && len > (size_t)padd && (!(buff[padd] = 0)))
		len = padd;
	if (padded_addr && flag & F_ZERO && val < 0)
		DBA(*padded_addr, '-', *buff, '0');
	return (len);
}

static size_t	ft_write_unsigned(char *buff, unsigned long long val,
		int padd, const int flag)
{
	size_t						len;
	const unsigned long long	diviser = GET_UNSIGNED_BASE_NUMBER(flag);

	len = ft_uintlen_base(val, flag);
	(void)ft_write_padding_prefix_integer(&buff, &len, padd, flag | F_POSITIVE);
	*buff-- = ft_get_base_char(val % diviser, flag);
	while (val /= diviser)
		*buff-- = ft_get_base_char(val % diviser, flag);
	if (padd > 0 && len > (size_t)padd)
	{
		buff[padd - 1] = 0;
		len = padd;
	}
	return (len);
}

size_t			ft_write_val_padd_ulli(char *tmpbuff,
		unsigned long long int val, int padd, const int flag)
{
	if (flag & F_08BIT)
		CHOOSE_SIGNED(tmpbuff, (char)val, padd, flag);
	if (flag & F_16BIT)
		CHOOSE_SIGNED(tmpbuff, (short)val, padd, flag);
	if (flag & F_48BIT)
		CHOOSE_SIGNED(tmpbuff, (long)val, padd, flag);
	if (flag & F_64BIT)
		CHOOSE_SIGNED(tmpbuff, (long long)val, padd, flag);
	if (flag & F_32BIT)
		CHOOSE_SIGNED(tmpbuff, (int)val, padd, flag);
	if (!(flag & F_NOERROR))
		ft_exit("received int type without width flag !\n");
	return (0);
}
