/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/30 16:27:10 by vdaviot           #+#    #+#             */
/*   Updated: 2016/08/30 16:27:13 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

static	void	get_mtl_one(char *line, t_mesh *mesh, int count)
{
	if (!ft_sscanf(LF_NEWMTL, line, &mesh->mtl[count].name))
		return ;
	else if (!(ft_sscanf(LF_NS, line, &mesh->mtl[count].ns)))
		return ;
	else if (!(ft_sscanf(LF_NI, line, &mesh->mtl[count].ni)))
		return ;
	else if (!(ft_sscanf(LF_D, line, &mesh->mtl[count].dissolution)))
		return ;
	else if (!(ft_sscanf(LF_TR, line, &mesh->mtl[count].tr)))
		return ;
	else if (!(ft_sscanf(LF_TF, line, &mesh->mtl[count].tf_rgb.x,
	 &mesh->mtl[count].tf_rgb.y, &mesh->mtl[count].tf_rgb.z)))
		return ;
	else if (!(ft_sscanf(LF_ILLUM, line, &mesh->mtl[count].illum)))
		return ;
	else if (!(ft_sscanf(LF_KA, line, &mesh->mtl[count].ka_rgb.x,
		&mesh->mtl[count].ka_rgb.y, &mesh->mtl[count].ka_rgb.z)))
		return ;
}

static	void	get_mtl_two(char *line, t_mesh *mesh, int count)
{
	if (!(ft_sscanf(LF_KD, line, &mesh->mtl[count].kd_rgb.x,
		&mesh->mtl[count].kd_rgb.y, &mesh->mtl[count].kd_rgb.z)))
		return ;
	if (!(ft_sscanf(LF_KS, line, &mesh->mtl[count].ks_rgb.x, 
		&mesh->mtl[count].ks_rgb.y, &mesh->mtl[count].ks_rgb.z)))
		return ;
	if (!(ft_sscanf(LF_KE, line, &mesh->mtl[count].ke_rgb.x, 
		&mesh->mtl[count].ke_rgb.y, &mesh->mtl[count].ke_rgb.z)))
		return ;
	if (!(ft_sscanf(LF_MAP_KA, line, &mesh->mtl[count].map_ka)))
		return ;
	if (!(ft_sscanf(LF_MAP_KD, line, &mesh->mtl[count].map_kd)))
		return ;
	if (!(ft_sscanf(LF_MAP_REFL, line, &mesh->mtl[count].map_refl)))
		return ;
}

void			parse_mtl(t_mesh *mesh)
{
	int 		fd;
	int			count;
	char		line[0xF000];

	count = -1;
	if ((fd = open(mesh->mtl_file, O_RDONLY)) == -1)
		return ;
	while(gl(line, &fd))
	{
		if (ft_strstr(line, "newmtl"))
			count++;
		get_mtl_one(line, mesh, count);
		get_mtl_two(line, mesh, count);
	}
	close(fd);
}






















