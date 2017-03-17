/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trimnm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/24 22:42:11 by vdaviot           #+#    #+#             */
/*   Updated: 2016/08/24 22:42:12 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

void		ft_trimnomalloc(char *origin, char *dest)
{
	char	*tmp;

	if (!(origin || !dest))
		return ;
	tmp = ft_strtrim(origin);
	ft_strcpy(dest, tmp);
	ft_strdel(&tmp);
	// ft_strdel(&origin);
}