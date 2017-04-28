/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 05:53:41 by pmartine          #+#    #+#             */
/*   Updated: 2017/04/28 06:50:22 by pmartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "parser.h"
#define ISTYPE(x) (obj->primitive.type == x + 1)
#define ISPRIMITIVE (ISTYPE(SPHERE) || ISTYPE(PLANE) || ISTYPE(CYLINDRE) || ISTYPE(CONE) || ISTYPE(CUBE) || ISTYPE(GLASS))
#define ISLIGHT (ISTYPE(POINT_LIGHT) || ISTYPE(DIRECTIONAL_LIGHT) || ISTYPE(SPOT_LIGHT))


struct	s_hit
{
	char	*objname;
	float	dist;
}				t_hit;

struct	s_ray
{
	vec3	pos;
	vec3	dir;
}				t_ray;

float			dot(vec3 v1, vec3 v2)
{
	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_hit			plane(t_object *obj)
{
	t_hit		h;
	float		t;
	t = (dot(norm,pos) - (dot (norm, r.pos))) / dot (norm, r.dir);

	if (t < h.dist)
	{
			h.dist = t;
			h.objname = ft_strcpy(h.objname, obj.name);
  }
	return (h);
}

t_hit			sphere(t_object *obj)
{
	t_hit		h;

}

t_hit			raytrace(t_scene *scene)
{
	int				i;
	int				nb_obj;
	t_object	*obj;
	t_hit				h;
	t_hit				hit;
	t_ray				ray;

	h.dist = 1e20;
	ray.pos = ;
	ray.dir = ;

	i = -1;
	nb_obj = scene.nb_object;
	while (++i < nb_obj)
	{
		obj = scene.root_view;
		if (ISTYPE(PLANE))
			hit = plane(obj);
		else
			hit = sphere(obj);
		if (hit.dist < h.dist)
			h = hit;
	}
}

void		onClick()
{
	t_scene		*scene = get_scene(NULL);
	t_hit				h;

	g_forward;
	g_move;
	g_mouse;

	h.dist = 1e20;
	h = raytrace(scene);

	if (h.dist < 1e20)
	{
		g_selected_object_pos = glGetUniformLocation(get_program(), h->objname + "_position");
		g_selected_object_dir = glGetUniformLocation(get_program(), h->objname + "_rotation");
		g_selected_position = object position;
		g_selected_rotation = object rotation;
	}
	g_selected_object_pos = -1;
	g_selected_object_dir = -1;
}
