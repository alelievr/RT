/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/22 17:04:13 by vdaviot           #+#    #+#             */
/*   Updated: 2016/08/22 17:04:14 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

double	ft_atod(char *str)
{
	double	value;
	int		sign;
	int		i;

	value = 0;
	sign = (*str == '-') ? -1 : 1;
	if (*str == '-' || *str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		value = value * 10 + (*str - '0');
		str++;
	}
	if (*str == '.')
	{
		str++;
		i = 1;
		while (ft_isdigit(*str))
		{
			value += (double)(*str - '0') / pow(10, i++);
			str++;
		}
	}
	str += 2;
	return (value * sign);
}
