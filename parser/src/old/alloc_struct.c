/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/12 18:38:39 by vdaviot           #+#    #+#             */
/*   Updated: 2017/01/18 01:02:28 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"
#define INFO infos[i]
#define SC_READ int fd; int	ret; char buf[0xf000]

void		alloc_struct(t_mesh **mesh, char *file, t_infos *infos)
{
	int		i;
	int		count;

	count = get_count(file, &infos);
	if (!(*mesh = (t_mesh*)malloc(sizeof(t_mesh) * count)))
		ft_exit("Malloc Error");
	i = -1;
	while (++i <= count)
	{
		ALIAS(m, (*mesh) + i);
		if (!(m->vertex = (t_vec3*)malloc(sizeof(t_vec3) * INFO.nb_v))
			|| !(m->normale = (t_vec3*)malloc(sizeof(t_vec3) * INFO.nb_vn))
			|| !(m->suv = (t_vec2*)malloc(sizeof(t_vec2) * INFO.nb_vt))
			|| !(m->triangles = (int*)malloc(sizeof(int) * (INFO.nb_f4 + INFO.nb_f * 3)))
			|| !(m->vnorm = (int*)malloc(sizeof(int) * (INFO.nb_f4 + INFO.nb_f * 3)))
			|| !(m->vtex = (int*)malloc(sizeof(int) * (INFO.nb_f4 + INFO.nb_f * 3)))
			|| !(m->mtl_tab = (char**)malloc(sizeof(char*) *
				INFO.nb_mat)))
			ft_exit("Malloc Erroor");
	}
}
/*
void		alloc_struct2(t_mesh **mesh, char *file)
{
	int		i;
	int		count;
	bool	allocation;
	SC_READ;

	allocation = true;
	if ((fd = open(file, O_RDONLY)) == -1)
	while (allocation)
	{
		get_count();
		alloc_mesh();
	}
}*/
