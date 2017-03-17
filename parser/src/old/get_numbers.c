/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_numbers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/12 18:37:56 by vdaviot           #+#    #+#             */
/*   Updated: 2016/09/12 18:37:57 by vdaviot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RTv1.h"

static	int			count_slash(char *str)
{
	char	*ptr;
	int		count;

	count = 0;
	ptr = str;
	while (*ptr && ptr++)
		if (*ptr == '/')
			count++;
	return (count);
}

static	int	get_v(char *buf, int *countv, int *countvn, int *countvt)
{
	char		*ptr;

	if ((ptr = ft_strchr(buf, 'v')))
	{
		if (ptr[1] == 'n')
			*countvn += 1;
		else if (ptr[1] == 't')
			*countvt += 1;
		else
			*countv += 1;
		return (1);
	}
	return (0);
}

static	int	get_f(char *buf, int *counttria, int *counttmp)
{
	if ((ft_strstr(buf, "f ")))
	{
		if ((count_slash(buf)) == 8)
			*counttmp += 1;
		else
			*counttria += 1;
		return (1);
	}
	return (0);
}

static	int	get_mtl(char *buf, int *countmat, int *countmtl)
{
	if ((ft_strstr(buf, "usemtl")))
	{
		*countmat += 1;
		return (1);
	}
	else if (ft_strstr(buf, "newmtl"))
	{
		*countmtl += 1;
		return (1);
	}
	return (0);
}

static	int	get_obj(char *buf, int *countgroup, int *countobj)
{
	char		*ptr;

	(void)countobj;
	if ((ptr = ft_strchr(buf, 'g')) && ptr[1] == ' ')
	{
		*countgroup += 1;
		return (1);
	}
	return (0);
}

static	int	set_infos(int **sizes, t_infos **infos, int i)
{
	int			p;
	int			d;
	int			count;

	count = 0;	
	p = -1;
	d = 0;
	if (!(*infos = (t_infos*)malloc(sizeof(t_infos) * i)))
		ft_exit("Malloc Error");
	while (++p < i)
	{
		ALIAS(info, (*infos) + p);
		info->nb_v = (size_t)sizes[0][p];
		info->nb_vn = (size_t)sizes[1][p];
		info->nb_vt = (size_t)sizes[2][p];
		info->nb_f = (size_t)sizes[3][p];
		info->nb_f4 = (size_t)sizes[4][p];
		info->nb_mtl = (size_t)sizes[7][p];
		info->nb_mat = (size_t)sizes[8][p];
		count++;
	}
	return (i);
}

static	int		alloc_count(int **count, int nb)
{
	int			i;

	i = -1;
	while (++i < 9)
		if (!(count[i] = (int*)malloc(sizeof(int) * nb)))
			ft_exit("Malloc Error");
	return (nb);
}

static	int		get_nb_obj(char *file)
{
	int			count;
	int			fd;
	char		line[0xF000];

	count = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open error");
	while (gl(line, &fd))
		if (ft_strstr(line, "# obj"))
			count++;
	close(fd);
	if (count == 0)
		return (1);
	return (count);
}

int				get_count(char *file, t_infos **infos)
{
	int			fd;
	int			**count;
	int			obj;
	char		buf[0xF000 + 1];

	obj = 0;
	if (!(count = (int**)malloc(sizeof(int*) * 9)))
		ft_exit("Malloc Error");
	if (alloc_count(count, get_nb_obj(file)) == 1)
		obj++;
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open Error");
	while (gl(buf, &fd))
	{
		if (ft_strstr(buf, "object"))
			obj++;
		else if (buf[0] == '#')
			continue ;
		else if ((get_v(buf, &count[0][obj - 1], &count[1][obj - 1], &count[2][obj - 1])) == 1
			|| (get_f(buf, &count[3][obj - 1], &count[4][obj - 1])) == 1 
			|| (get_obj(buf, &count[5][obj - 1], &count[6][obj - 1])) == 1
			|| (get_mtl(buf, &count[7][obj - 1], &count[8][obj - 1])) == 1)
			;
	}
	close(fd);
	return (set_infos(count, infos, obj));
}
