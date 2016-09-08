/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 21:17:31 by alelievr          #+#    #+#             */
/*   Updated: 2016/05/24 20:56:46 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sprintf.h"
#include <stdarg.h>

static t_type g_typesize = {
	{T_CHAR, sizeof(char)},
	{T_SHORT, sizeof(short)},
	{T_INTEGER, sizeof(int)},
	{T_LONG, sizeof(long)},
	{T_STRING, sizeof(char *)},
};

void	get_next_arg(va_list *ap, int type, void *data)
{
	*(unsigned long int *)data = va_arg(*ap, g_typesize[type]);
}
