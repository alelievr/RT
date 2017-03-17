/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingerino.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/10 17:44:07 by vdaviot           #+#    #+#             */
/*   Updated: 2016/09/10 17:44:09 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"


static	int		get_mtl_file(char *line, t_mesh *mesh, bool reset)
{
	char		buf[MAX_STR];
	static	int	p = 0;

	if (reset == true)
		p = 0;
	if (!ft_sscanf(LF_MTL, line, mesh->mtl_file))
	{
		ft_strcpy(buf, FILES_PATH);
		ft_strlcat(buf, mesh->mtl_file, MAX_STR);
		ft_strcpy(mesh->mtl_file, buf);
		return (1);
	}
	else if (!ft_sscanf(LF_USEMTL, line, buf))
	{
		mesh->mtl_tab[p] = ft_strdup(buf);
		p++;
		return (1);
	}
	return (0);
}

static	int		get_v(char *line, t_mesh *mesh, bool reset)
{
	static int		p = 0;
	static int		t = 0;
	static int		r = 0;

	ALIAS(m, mesh);
	if (reset == true)
	{
		p = 0;
		r = 0;
		t = 0;
	}
	if (!ft_sscanf(LF_V, line, &m->vertex[p].x, &m->vertex[p].y,
	 &m->vertex[p].z))
	{
		p += 1;
		return (1);
	}
	else if (!ft_sscanf(LF_VN, line, &m->normale[t].x, &m->normale[t].y,
		&m->normale[t].z))
	{
		t += 1;
		return (1);
	}
	else if (!ft_sscanf(LF_VT, line, &m->suv[r].x, &m->suv[r].y))
	{
		r += 1;
		return (1);
	}
	else if (!ft_sscanf(LF_VT2, line, &m->suv[r].x, &m->suv[r].y, (float[1]){0}))
	{
		r += 1;
		return (1);
	}
	return (0);
}

static	void		set_fshit(t_mesh *mesh, int *p, int *t, int *r, int *tmp)
{
	mesh->triangles[*p + 3] = tmp[0];
	mesh->triangles[*p + 4] = mesh->triangles[*p];
	mesh->triangles[*p + 5] = mesh->triangles[*p + 1];
	mesh->vtex[*t + 3] = tmp[1];
	mesh->vtex[*t + 4] = mesh->vtex[*t];
	mesh->vtex[*t + 5] = mesh->vtex[*t + 1];
	mesh->vnorm[*r + 3] = tmp[2];
	mesh->vnorm[*r + 4] = mesh->vnorm[*r];
	mesh->vnorm[*r + 5] = mesh->vnorm[*r + 1];
}

static	int			get_f(char *line, t_mesh *mesh, bool reset)
{
	static int		p = 0;
	static int		t = 0;
	static int		r = 0;
	int				*tmp;

	ALIAS(v, mesh->triangles);
	ALIAS(vt, mesh->vtex);
	ALIAS(vn, mesh->vnorm);
	tmp = (int*)(int[3]){0};
	if (reset == true)
	{
		p = 0;
		t = 0;
		r = 0;
	}
	if (!ft_sscanf(LF_F3, line, &v[p], &vt[t], &vn[r], &v[p + 1],
	 &vt[t + 1], &vn[r + 1], &v[p + 2], &vt[t + 2], &vn[r + 2]))
	{
		p += 2;
		t += 2;
		r += 2;
		return (1);
	}
	else if (!ft_sscanf(LF_F4, line, &v[p], &vt[t], &vn[r], &v[p + 1],
	 &vt[t + 1], &vn[r + 1], &v[p + 2], &vt[t + 2], &vn[r + 2],
	 &tmp[0], &tmp[1], &tmp[2]))
	{
		set_fshit(mesh, &p, &t, &t, tmp);
		p += 5;
		t += 5;
		r += 5;
		return (1);
	}
	return (0);
}

static	void		set_ffourshit(t_mesh *mesh, int *p, int *t, int *r, int *tmp)
{
	MT[*p + 3] = tmp[0];
	MT[*p + 4] = MT[*p];
	MT[*p + 5] = MT[*p + 1];
	MV[*t] = 0;
	MV[*t + 1] = 0;
	MV[*t + 2] = 0;
	MV[*t + 3] = 0;
	MV[*t + 4] = 0;
	MV[*t + 5] = 0;
	MN[*r + 3] = tmp[1];
	MN[*r + 4] = MN[*r];
	MN[*r + 5] = MN[*r + 1];
}

static	int			get_ffour(char *line, t_mesh *mesh, bool reset)
{
	static int		p = 0;
	static int		t = 0;
	static int		r = 0;
	int				*tmp;

	tmp = (int*)(int[2]){0};
	if (reset == true)
	{
		p = 0;
		t = 0;
		r = 0;
	}
	if (!ft_sscanf(LF_F4S, line, &MT[p], &MN[t], &MT[p + 1],
	 &MN[t + 1], &MT[p + 2], &MN[t + 2], &tmp[0], &tmp[1]))
	{
		set_ffourshit(mesh, &p, &t,	&r,	tmp);
		p += 5;
		t += 5;
		r += 5;
		return (1);
	}
	return (0);
}

void				parse_obj_file(char *file, t_mesh **mesh)
{
	int				fd;
	static	char	line[0xF000];
	int				i;
	bool			reset;
	int				count;

	i = 0;
	count = 0;
	reset = false;
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Non viable file given, exiting.");
	while (gl(line, &fd))
	{
		ALIAS(m, *mesh + i);
		if (ft_strstr(line, "# obj"))
		{
			i++;
			reset = true;
		}
		else if (line[0] == '#')
			continue ;
		if ((get_mtl_file(line, m, reset)) == 1 || (get_v(line, m, reset)) == 1
			|| (get_f(line, m, reset)) == 1 || (get_ffour(line, m, reset)) == 1)
		{
			reset = false;
			continue ;
		}
	}
}