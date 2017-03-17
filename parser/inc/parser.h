/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RTv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaviot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 14:47:27 by vdaviot           #+#    #+#             */
/*   Updated: 2017/03/17 02:59:42 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <limits.h>
# include <stdarg.h>
# include <stdbool.h>

/*
	.RT format defines
*/

# define LF_RT_NAME "\\sname:\\s%s\\s"
# define LF_RT_TYPE "\\stype:\\s%s\\s"
# define LF_RT_POS "\\spos:\\s%f\\s%f\\s%f\\s"
# define LF_RT_DIR "\\sdir:\\s%f\\s%f\\s%f\\s"
# define LF_RT_SCALE "\\ssize:\\s%f\\s%f\\s%f\\s"
# define LF_RT_COLOR "\\scolor:\\s%s\\s"
# define LF_RT_MAT "\\smat:\\s%s\\s"
# define LF_RT_MTL "\\smtl:\\sfiles/%s\\s"
# define LF_RT_AMBIANCE "\\sambiance:\\s%s\\s%f\\s"
# define LF_RT_EFFECT "\\seffect:\\s%f\\s%f\\s%f\\s%f\\s%f\\s"
# define LF_RT_RADIUS "\\sradius:\\s%d\\s"
# define LF_RT_PDIST "\\spdist:\\s%f\\s"
# define LF_RT_GATE_XOR "\\scsg:\\s%s\\s"
# define LF_RT_GATE_OR
# define LF_RT_GATE_AND
# define LF_RT_GATE_NAND
/*
	.OBJ format defines
*/

# define LF_V "\\sv\\s%f\\s%f\\s%f\\s"
# define LF_VN "\\svn\\s%f\\s%f\\s%f\\s"
# define LF_VT "\\svt\\s%f\\s%f\\s"
# define LF_VT2 "\\svt\\s%f\\s%f\\s%f\\s"
# define LF_F3 "\\sf\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s"
# define LF_F4 "\\sf\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s%d\\s"
# define LF_F4S "\\sf\\s%d//%d\\s%d//%d\\s%d//%d\\s%d//%d\\s"
# define LF_MTL "\\smtllib\\s%s\\s"
# define LF_USEMTL "\\susemtl\\s%s\\s"

/*
	.MTL format defines
*/

# define LF_NEWMTL "\\snewmtl\\s%s\\s"
# define LF_NS "\\sNs\\s%f\\s"
# define LF_NI "\\sNi\\s%f\\s"
# define LF_D "\\sd\\s%f\\s"
# define LF_TR "\\sTr\\s%f\\s%f\\s%f\\s"
# define LF_TF "\\sTf\\s%f\\s%f\\s%f\\s"
# define LF_ILLUM "\\sillum\\s%d\\s"
# define LF_KA "\\sKa\\s%f\\s%f\\s%f\\s"
# define LF_KD "\\sKd\\s%f\\s%f\\s%f\\s"
# define LF_KS "\\sKs\\s%f\\s%f\\s%f\\s"
# define LF_KE "\\sKe\\s%f\\s%f\\s%f\\s"
# define LF_MAP_KA "\\smap_Ka\\s%s\\s"
# define LF_MAP_KD "\\smap_Kd\\s%s\\s"
# define LF_MAP_REFL "\\smap_refl\\s%s\\s"

/*
	Other
*/

typedef	struct 			s_vec3
{
	float				x;
	float				y;
	float				z;
}						t_vec3;

typedef	struct			s_vec2
{
	float				x;
	float				y;
}						t_vec2;

typedef	struct			s_vec4
{
	float				x;
	float				y;
	float				z;
	float				w;
}						t_vec4;

enum		e_primitive_type
{
	SPHERE,
	PLANE,
	CYLINDRE,
	CONE,
	CAMERA,
	POINT_LIGHT,
	SPOT_LIGHT,
};

enum					e_post_processing
{
	SEPIA =				0x0001,
	BLACK_AND_WHITE =	0x0002,
	CARTOON =			0x0004,
	ANTIALIASING =		0x0008,
	DEPTH_OF_FIELD =	0x0010,
	BLOOM =				0x0020,
	LENS_FLARE =		0x0040,
	ACES_TONEMAPPING =	0x0080,
	VIGNETTING =		0x0100,
	DALTONIZE =			0x0200,
	NIGHT_VISION =		0x0400,
};

enum					e_illumination
{
	AMBIANT =			0x001,
	CAST_SHADOW =		0x002,
	RECEIVE_SHADOW =	0x004,
};

typedef	struct  s_primitive
{
	int					type;
	union {
		float			radius;
		float			height;
		float			angle;
	};
	t_vec4				*slice;
	int					nb_slice;
}				t_primitive;

typedef struct			s_transform
{
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		scale;
}						t_transform;

typedef struct			s_image
{
	char			file[1024];
	int				opengl_id;
}						t_image;

typedef	struct 			s_material
{
	char				name[256];
	int					illum;
	t_vec3				color;
	t_vec3				emission_color;
	t_vec3				highlight_color;
	float				transparency;
	float				specular;
	float				reflection;
	float				refraction;
	t_image				texture;
	t_image				bumpmap;
}						t_material;

typedef struct			s_submesh
{
	char			name[128];
	t_material		material;
	t_vec3			*vertex;
	t_vec3			*normals;
	t_vec2			*uvs;
	int				*triangles;
	union {
		int				nb_vertex;
		int				nb_normals;
		int				nb_uvs;
	};
	int				nb_triangles;
}						t_submesh;

typedef	struct			s_mesh
{
	t_submesh		*submeshs;
	int				nb_submesh;
}						t_mesh;

typedef struct			s_camera
{
	float		fov;

	int			post_processing_mask;
}						t_camera;

typedef struct			s_light
{
	int			color;
	float		intensity;
	float		angle;
}						t_light;

typedef struct s_object	t_object;

typedef struct			s_object
{
	char		name[256];
	int			indent_level;
	t_transform	transform;
	union {
		t_material	material;
		t_light		light_prop;
		t_camera	camera;
	};
	union {
		t_mesh		mesh;
		t_primitive	primivite;
	};
	bool		is_primitive;
	t_object	*children;
	t_object	*parent;
	t_object	*brother_of_children;
}						t_object;

typedef struct			s_scene
{
	t_object		root_view;
}						t_scene;

/*
	Parsing
*/

void					parse_obj(char *file, t_mesh **mesh);
void					parse_mtl(t_mesh *mesh);
int						ft_sscanf(char *format, char *str, ...);
int						gl(char *dst, int *fd);
void					parse_rt_file(char *file, t_scene *prim);
void					parse_obj_file(char *file, t_mesh *mesh);

#endif
