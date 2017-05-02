/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <pmartine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/02 21:54:21 by pmartine          #+#    #+#             */
/*   Updated: 2017/05/03 00:22:17 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "keywords.h"

#define NO4(var, n)(scene->root_view, n)) ft_exit("name already exist: %s\n",n);
#define NO3(var, n)if (c != NULL && name_already_exists NO4(var, n)
#define NO2(var, n)strcpy(var->name, format_name(n)); NO3(var, n)
#define NO1(var, n)sizeof(t_object)); init_default_object(var); NO2(var, n)
#define NO(var, n)if (!var) ft_exit("malloc error"); bzero(var, NO1(var, n)
#define NEW_OBJECT(var, n) var = (t_object *)malloc(sizeof(t_object)); NO(var,n)
#define SS(t) while(*t&&ft_isspace(*t))t++;
#define SKIP_EMPTY_LINE(l) {char*t=l;SS(t);if (*t==0)continue;}
#define FP4(X, line) fill_prop_vec4) (X, line)
#define FP3(X, line)t_primitive *:fill_prop_primitive, t_vec4 *: FP4(X,line)
#define FP2(X, line)fill_prop_material, t_camera *:fill_prop_camera, FP3(X,line)
#define FP1(X, line)t_transform *: fill_prop_transform, t_material *:FP2(X,line)
#define FILL_PROP(X, line) _Generic((X), t_light *: fill_prop_light, FP1(X,line)
#define A(c, line, p1) FILL_PROP(&c-> p1, line);

static char		*format_name(char *name)
{
	char	*ret;

	ret = name;
	while (*name)
	{
		if (*name == ':')
		{
			*name = 0;
			break ;
		}
		if (!isalnum(*name))
			*name = '_';
		name++;
	}
	return (ret);
}

bool			check_obj_line(char *line, char *obj_name, int *indent_level)
{
	char	*k;
	int		space_number;

	space_number = 0;
	(*indent_level) = 0;
	while (*line && (*line == '\t' || *line == ' '))
	{
		if (*line == '\t')
			(*indent_level)++;
		else if (*line == ' ')
			space_number++;
		if (space_number == 4 && !(space_number = 0))
			(*indent_level)++;
		line++;
	}
	if (!*line)
		return (false);
	k = line;
	while (*line && *line++ != ':')
		;
	if (!*line)
	{
		if (FOREACH(g_restricted_keywords, keyword))
		{
			if (!ft_strcmp(k, keyword))
				return (false);
		}
		ft_strlcpy(obj_name, k, 256);
		return (true);
	}
	else
		return (false);
}

static void		init_default_object(t_object *o)
{
	o->light_prop.color = (t_vec3){255, 255, 255};
	o->material.color = (t_vec3){255, 0, 255};
	o->material.specular = 0;
	o->material.opacity = 1;
	o->material.refraction = 1;
}

void			fill_prop_vec4(t_vec4 *data, char *line)
{
	char	word[256];
	float	radius;
	float	speed;

	if (!ft_sscanf(LF_RT_MOVE, line, word, 256, &radius, &speed))
	{
		if (strchr(word, 'X'))
			data->x = radius;
		if (strchr(word, 'Y'))
			data->y = radius;
		if (strchr(word, 'Z'))
			data->z = radius;
		data->w = speed;
	}
	if (!ft_sscanf(LF_RT_ROTATE, line, word, 256, &speed))
	{
		if (strchr(word, 'X'))
			data->x = 1;
		if (strchr(word, 'Y'))
			data->y = 1;
		if (strchr(word, 'Z'))
			data->z = 1;
		data->w = speed;
	}
}

void			fill_prop_camera(t_camera *cam, char *line)
{
	int		ret;
	char	*str;
	char	word[256];
	int		i;

	str = (char *)(char[256]){0};
	ret = 0;
	ft_sscanf(LF_RT_FOV, line, &cam->fov);
	ft_sscanf(LF_RT_AMBIENT, line, &cam->ambient);
	if (!ft_sscanf(LF_RT_MASK, line, str, 256))
	{
		printf("line: %s\n", line);
		while ((get_next_word(&str, word)))
		{
			i = 0;
			while (ft_strcmp(g_mask_restricted_keywords[i].name, "END"))
			{
				if (!ft_strcmp(g_mask_restricted_keywords[i].name, word))
					ret |= g_mask_restricted_keywords[i].value;
				i++;
			}
		}
		printf("post processing mask: %i\n", ret);
		cam->post_processing_mask = ret;
	}
}

void			fill_prop_primitive(t_primitive *p, char *line)
{
	char	*str;
	int		i;

	str = (char *)(char[256]){0};
	ft_sscanf(LF_RT_RADIUS, line, &p->radius);
	ft_sscanf(LF_RT_HEIGHT, line, &p->height);
	ft_sscanf(LF_RT_ANGLE, line, &p->angle);
	if (!ft_sscanf(LF_RT_TYPE, line, str, 256))
		if (FOR(i = 0, g_type_restricted_keywords[i], i++))
		{
			if (!ft_strcmp(g_type_restricted_keywords[i], ft_strupcase(str)))
				p->type = i + 1;
		}
	if (!p->type)
		ft_exit("mauvais type pour un objet\n");
	if (p->nsl < 5 && !ft_sscanf(LF_RT_SLICE, line, &p->slice[p->nsl].x,\
		&p->slice[p->nsl].y, &p->slice[p->nsl].z, &p->slice[p->nsl].w))
		p->nsl++;
}

void			fill_prop_light(t_light *l, char *line)
{
	if (ft_sscanf(LF_RT_COLOR_F, line, &l->color.x, &l->color.y, &l->color.z))
		ft_sscanf(LF_RT_COLOR_V, line, &l->color.x, &l->color.y, &l->color.z);
	ft_sscanf(LF_RT_INTENSITY, line, &l->intensity);
}

void			fill_prop_transform(t_transform *t, char *line)
{
	if (!ft_sscanf(LF_RT_POS, line, &t->position.x, &t->position.y,\
		&t->position.z))
		t->initial_position = t->position;
	ft_sscanf(LF_RT_ROT, line, &t->euler_angles.x, &t->euler_angles.y,\
		&t->euler_angles.z);
}

void			fill_prop_material_map(t_material *mtl, char *line, char *word, char *str)
{
	int		i;
	int		ret;

	ret = 0;
	str = (char *)(char[256]){0};
	if (FOR(i = 0, i < 2, i++))
	{
		struct {char *fmt; t_image *img; bool *active;} maps[8] = {
			{LF_RT_BUMPMAP, &mtl->bumpmap, &mtl->has_bumpmap},
			{LF_RT_TEXTURE, &mtl->texture, &mtl->has_texture},
			{LF_RT_EMISSION_MAP, &mtl->texture, &mtl->has_emission_map},
			{LF_RT_HIGHLIGHT_MAP, &mtl->emission_map, &mtl->has_highlight_map},
			{LF_RT_OPACITY_MAP, &mtl->opacity_map, &mtl->has_opacity_map},
			{LF_RT_SPECULAR_MAP, &mtl->specular_map, &mtl->has_specular_map},
			{LF_RT_REFLECTION_MAP, &mtl->reflection_map, &mtl->has_reflection_map},
			{LF_RT_REFRACTION_MAP, &mtl->refraction_map, &mtl->has_refraction_map},
		};
		if (ft_sscanf(maps[i].fmt, line, maps[i].img->file, 1024))
			*(maps[i].active) = true;
	}
	if (!ft_sscanf(LF_RT_ILLUM, line, str, 256))
	{
		while (get_next_word(&str, word))
			if (FOR(i = 0, g_illum_restricted_keywords[i].value != END, i++))
				if (!ft_strcmp(g_illum_restricted_keywords[i].name, word))
					ret |= g_illum_restricted_keywords[i].value;
		mtl->illum = ret;
	}
}

void			fill_prop_material_effect(t_material *mtl, char *line)
{
	char	word[256];
	int		i;

	if (!ft_sscanf(LF_RT_COLOR_EFFECT, line, word, 256))
	{
		if (FOR(i = 0, g_color_effect_r_k[i].value != END, i++))
			if (!ft_strcmp(g_color_effect_r_k[i].name, word))
				mtl->color_effect = g_color_effect_r_k[i].value + 1;
	}
	if (!ft_sscanf(LF_RT_NORMAL_EFFECT, line, word, 256))
	{
		if (FOR(i = 0, g_normal_effect_r_k[i].value != END, i++))
			if (!ft_strcmp(g_normal_effect_r_k[i].name, word))
				mtl->normal_effect = g_normal_effect_r_k[i].value + 1;
	}
}

void			fill_prop_material(t_material *mtl, char *line)
{
	char	*str;
	char	word[256];
	int		ret;
	int		i;

	str = (char *)(char[256]){0};
	ret = 0;
	i = 0;
	PALIAS(mtl->color, c);
	PALIAS(mtl->emission_color, e);
	PALIAS(mtl->highlight_color, h);
	if (ft_sscanf(LF_RT_COLOR_F, line, &c->x, &c->y, &c->z))
		ft_sscanf(LF_RT_COLOR_V, line, &c->x, &c->y, &c->z);
	if (!ft_sscanf(LF_RT_EMISSION_COLOR_F, line, &e->x, &e->y, &e->z))
		ft_sscanf(LF_RT_EMISSION_COLOR_V, line, &e->x, &e->y, &e->z);
	if (!ft_sscanf(LF_RT_HIGHLIGHT_COLOR_F, line, &h->x, &h->y, &h->z))
		ft_sscanf(LF_RT_HIGHLIGHT_COLOR_V, line, &h->x, &h->y, &h->z);
	fill_prop_material_effect(mtl, line);
	ft_sscanf(LF_RT_OPACITY, line, &mtl->opacity);
	ft_sscanf(LF_RT_SPECULAR, line, &mtl->specular);
	ft_sscanf(LF_RT_REFLECTION, line, &mtl->reflection);
	ft_sscanf(LF_RT_REFRACTION, line, &mtl->refraction);
	fill_prop_material_map(mtl, line, word, str);
}

void			display_objects(t_object *lst_obj)
{
	while (lst_obj)
	{
		if (lst_obj->children)
			display_objects(lst_obj->children);
		lst_obj = lst_obj->brother_of_children;
	}
}

bool		name_already_exists(t_object *obj, char *name)
{
	if (obj != NULL)
		return (false);
	while (obj)
	{
		if (!ft_strcmp(obj->name, name))
			return (true);
		obj = obj->brother_of_children;
	}
	return (false);
}

void			parse_rt_file(char *file, t_scene *scene)
{
	int			fd;
	char		line[0xF000 + 1];
	char		obj_name[256];
	int			indent_level;
	int			nb_object;
	t_object	*c;
	t_object	*o;

	c = NULL;
	nb_object = 0;
	INIT(int, line_count, 0);
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_exit("Open Failed: %s\n", file);
	if (!file_is_regular(fd))
		ft_exit("bad file: %s\n", file);
	while (gl(line, &fd))
	{
		SKIP_EMPTY_LINE(line);
		if (check_obj_line(line, obj_name, &indent_level))
		{
			NEW_OBJECT(c, obj_name);
			c->indent_level = indent_level;
			scene->nb_object = ++nb_object;
			if (scene->nb_object == 1)
			{
				scene->root_view = c;
				o = c;
			}
			else if (o->indent_level < c->indent_level)
			{
				o->children = c;
				c->parent = o;
			}
			while (o->indent_level > c->indent_level)
				o = o->parent;
			if (scene->nb_object != 1 && o->indent_level == c->indent_level)
			{
				o->brother_of_children = c;
				c->parent = o->parent;
			}
			o = c;
			continue ;
		}
		if (indent_level >= 2)
			ft_exit("max indentation reached at line: %i\n", line_count);
		if (nb_object > 300)
			ft_exit("nb max object reached at line: %i\n", line_count);
		if (c == NULL)
			ft_exit("unexpected property without object: %s\n", line);
		if (indent_level == c->indent_level + 1)
		{
			A(c, line, primitive);
			A(c, line, transform);
			A(c, line, move);
			A(c, line, rotate);
			if (c->primitive.type >= 11)
				A(c, line, light_prop);
			if (c->primitive.type == 10)
				A(c, line, camera);
			if (c->primitive.type <= 9)
				A(c, line, material);
		}
		else
			ft_exit("bad indentation at line %i\n", line_count);
	}
	display_objects(scene->root_view);
	close(fd);
}
