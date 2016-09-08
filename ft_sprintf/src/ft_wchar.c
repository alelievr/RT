/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wchar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/24 22:07:03 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/24 22:42:55 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"

static void		ft_write_val_padd_wchar_core(char *tmpbuff, unsigned int ch,
		size_t *ret, int n)
{
	if (n > 7)
	{
		if (n > 11)
		{
			if (n > 16)
			{
				tmpbuff[(*ret)++] = ((ch >> 18) & 7) | 240;
				tmpbuff[(*ret)++] = ((ch >> 12) & 63) | 128;
			}
			else
				tmpbuff[(*ret)++] = ((ch >> 12) & 15) | 224;
			tmpbuff[(*ret)++] = ((ch >> 6) & 63) | 128;
		}
		else
			tmpbuff[(*ret)++] = ((ch >> 6) & 31) | 192;
		tmpbuff[(*ret)++] = (ch & 63) | 128;
	}
	else
		tmpbuff[(*ret)++] = ch;
}

size_t			ft_write_val_padd_wchar(char *tmpbuff, wchar_t val,
		int padd, const int flag)
{
	unsigned int	ch;
	int				n;
	size_t			ret;

	ret = 0;
	n = 1;
	ch = (unsigned int)val;
	while ((val >>= 1))
		n++;
	ft_write_val_padd_wchar_core(tmpbuff, ch, &ret, n);
	tmpbuff[ret] = 0;
	if (padd > 0 && ret > (size_t)padd)
		return (0);
	(void)flag;
	return (ret);
}

size_t			ft_write_val_padd_wchars(char *tmpbuff, wchar_t *val,
		int padd, const int flag)
{
	size_t	ret;
	size_t	tmp;

	ret = 0;
	while (*val)
	{
		tmp = ft_write_val_padd_wchar(tmpbuff, *val, padd, flag);
		if (tmp == 0)
		{
			*tmpbuff = 0;
			break ;
		}
		ret += tmp;
		padd -= tmp;
		tmpbuff += tmp;
		val++;
	}
	return (ret);
}

size_t			ft_concat(char *buff, wchar_t *wchrs, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	size_t	len;

	if (padd != NOPADD && padd < 0 && !(flag & F_NOERROR))
		ft_exit("invalid negative precision for string !\n");
	len = FT_WRITE_VAL_PADD(buff, (wchar_t *)wchrs, padd, flag);
	return (ft_concat_align(buff, buff, len, align, flag));
}

size_t			ft_concat(char *buff, double d, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	char	tmpbuff[0xF00];
	size_t	len;

	len = FT_WRITE_VAL_PADD(tmpbuff, d, padd, flag);
	return (ft_concat_align(buff, tmpbuff, len, align, flag));
}
