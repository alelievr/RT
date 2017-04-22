/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:57:18 by vdaviot           #+#    #+#             */
/*   Updated: 2017/04/22 19:36:35 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>

int				gl(char *dst, int *fd)
{
	static char buff[0xF000 + 1];
	static char *ptr = buff;
	char		*nl;
	int			r;

	r = 0;
	if ((long)ptr == 0x1l)
		ptr = buff;
	if (!(nl = ft_strchr(ptr, '\n')))
	{
		if (ptr != buff)
		{
			ft_strcpy(buff, ptr);
			ptr = buff + (sizeof(buff) - (ptr - buff)) - 1;
		}
		r = read(*fd, ptr, (sizeof(buff) - (ptr - buff) - 1));
		ptr[r] = '\0';
		ptr = buff;
	}
	if ((nl = ft_strchr(ptr, '\n')))
		ft_strlcpy(dst, ptr, nl - ptr + 1);
	ptr = nl + 1;
	return (!!nl);
}
