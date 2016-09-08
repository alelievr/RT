/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 20:35:36 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/24 22:42:16 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"

size_t		ft_concat_align(char *buff, char *tmpbuff, size_t len,
		ARG_II flag)
{
	const char	align_byte = ' ';
	size_t		alignsize;
	size_t		i;

	alignsize = 0;
	i = (size_t)-1;
	if (align > 0 && (len < (size_t)align) && align != NOALIGN)
	{
		ft_memmove(buff + ((size_t)align - len), tmpbuff, len + 1);
		alignsize = (size_t)align - len;
		while (++i < (size_t)align - len)
			*buff++ = align_byte;
	}
	else if (align < 0 && (len < (size_t)(-align)) && align != NOALIGN)
	{
		ft_memmove(buff, tmpbuff, len);
		buff += len;
		while (++i < (size_t)(-align) - len)
			*buff++ = align_byte;
		*buff = 0;
	}
	else
		ft_memmove(buff, tmpbuff, len + 1);
	(void)flag;
	return (alignsize + len);
}

size_t		ft_concat(char *buff, char c, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	char	tmpbuff[2];

	tmpbuff[0] = c;
	tmpbuff[1] = 0;
	(void)flag;
	(void)padd;
	return (ft_concat_align(buff, tmpbuff, 1, align, flag));
}

size_t		ft_concat(char *buff, unsigned long long int val, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	char	tmpbuff[24];
	size_t	len;

	len = FT_WRITE_VAL_PADD(tmpbuff, val, padd, flag);
	return (ft_concat_align(buff, tmpbuff, len, align, flag));
}

size_t		ft_concat(char *buff, char *str, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	size_t	len;

	if (padd != NOPADD && padd < 0 && !(flag & F_NOERROR))
		ft_exit("invalid negative precision for string !\n");
	len = ft_strlen(str);
	ft_strlcpy(buff, str, (padd != NOPADD && len > (size_t)padd)
			? padd + 1 : len);
	return (ft_concat_align(buff, buff, len, align, flag));
}

size_t		ft_concat(char *buff, wchar_t wch, int padd,
		ARG_II flag) __attribute__((overloadable))
{
	char	tmpbuff[5];
	size_t	len;

	len = FT_WRITE_VAL_PADD(tmpbuff, wch, padd, flag);
	if (padd != NOPADD && padd < 0 && !(flag & F_NOERROR))
		ft_exit("invalid negative padding on wchar !\n");
	return (ft_concat_align(buff, tmpbuff, len, align, flag));
}
