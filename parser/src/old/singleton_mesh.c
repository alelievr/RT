/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton_mesh.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/11 17:11:49 by vdaviot           #+#    #+#             */
/*   Updated: 2016/09/11 17:11:50 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

t_mesh		*singleton_mesh(void)
{
	static	t_mesh	mesh;
	t_mesh			*ptr;

	if (!mesh)
	{
		if (!(mesh = (t_mesh)malloc(sizeof(t_mesh))))
			ft_exit("Malloc error");
		ptr = mesh;
	}
	return (ptr);
}