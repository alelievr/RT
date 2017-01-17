/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_padd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/07 01:32:14 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/21 20:43:42 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"

#include <stdio.h>
size_t	ft_uintlen_base(unsigned long long int val, const int flag)
{
	size_t				ret;
	unsigned long long	diviser;

	if (val == 0)
		return (1);
	ret = 0;
	diviser = GET_UNSIGNED_BASE_NUMBER(flag);
	while (val && ++ret)
		val /= diviser;
	return (ret);
}

size_t	ft_intlen_base(long long int val, const int flag)
{
	size_t		ret;
	long long	diviser;

	if (!val)
		return (1);
	if (flag & F_OCTAL || flag & F_HEXA)
		return (ft_uintlen_base((unsigned long long)val, flag));
	ret = 0;
	diviser = GET_SIGNED_BASE_NUMBER(flag);
	if (val < 0)
		++ret;
	while (val && ++ret)
	{
		val /= diviser;
	}
	return (ret);
}

char	ft_get_base_char(int i, const int flag)
{
	char	ret;

	ret = 0;
	if (i < 10)
		ret = i + '0';
	else
		ret = i - 10 + 'a';
	if (i >= 10 && flag & F_UPPERCASE)
		ret -= 'a' - 'A';
	return (ret);
}

size_t	ft_add_integer_prefix(char *buff, const int flag)
{
	if (flag & F_OCTAL || flag & F_HEXA || flag & F_BINARY)
	{
		*buff++ = '0';
		if (flag & F_OCTAL)
			return (1);
		else if (flag & F_HEXA)
			*buff = (flag & F_UPPERCASE) ? 'X' : 'x';
		else if (flag & F_BINARY)
			*buff = (flag & F_UPPERCASE) ? 'B' : 'b';
		return (2);
	}
	return (0);
}
