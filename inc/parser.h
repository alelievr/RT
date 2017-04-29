/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avially <avially@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/08 14:47:27 by alelievr          #+#    #+#             */
/*   Updated: 2017/04/29 06:19:23 by avially          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shaderpixel.h"

# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdarg.h>
# include <stdbool.h>

/*
	.RT format defines
*/

# define LF_RT_POS		"\\spos:\\s%f\\s%f\\s%f\\s"
# define LF_RT_ROT		"\\srot:\\s%f\\s%f\\s%f\\s"
# define LF_RT_SCALE	"\\sscale:\\s%f\\s%f\\s%f\\s"

# define LF_RT_COLOR_F	"\\scolor:\\s%f\\s%f\\s%f\\s"
# define LF_RT_COLOR_V	"\\scolor:\\s%z\\s"
# define LF_RT_EMISSION_COLOR_F "\\semission color:\\s%f\\s%f\\s%f\\s"
# define LF_RT_EMISSION_COLOR_V "\\semission color:\\s%z\\s"
# define LF_RT_HIGHLIGHT_COLOR_F "\\shighlight color:\\s%f\\s%f\\s%f\\s"
# define LF_RT_HIGHLIGHT_COLOR_V "\\shighlight color:\\s%z\\s"

# define LF_RT_OPACITY "\\sopacity:\\s%f\\s"
# define LF_RT_SPECULAR "\\sspecular:\\s%f\\s"
# define LF_RT_REFLECTION "\\sreflection:\\s%f\\s"
# define LF_RT_REFRACTION "\\srefraction:\\s%f\\s"
# define LF_RT_INTENSITY "\\sintensity:\\s%f\\s"
# define LF_RT_ANGLE "\\sangle:\\s%f\\s"
# define LF_RT_RADIUS "\\sradius:\\s%f\\s"
# define LF_RT_HEIGHT "\\sheight:\\s%f\\s"

# define LF_RT_FOV "\\sfov:\\s%f\\s"
# define LF_RT_AMBIENT "\\sambient:\\s%f\\s"

# define LF_RT_SLICE "\\sslice:\\s%f\\s%f\\s%f\\s%f\\s"

# define LF_RT_TEXTURE	"\\stexture:\\s%s\\s"
# define LF_RT_BUMPMAP	"\\sbumpmap:\\s%s\\s"
# define LF_RT_EMISSION_MAP	"\\semission color map:\\s%s\\s"
# define LF_RT_HIGHLIGHT_MAP "\\shighlight color map:\\s%s\\s"
# define LF_RT_REFRACTION_MAP	"\\srefraction map:\\s%s\\s"
# define LF_RT_OPACITY_MAP "\\sopacity map:\\s%s\\s"
# define LF_RT_SPECULAR_MAP "\\sspecular map:\\s%s\\s"
# define LF_RT_REFLECTION_MAP "\\sreflection map map:\\s%s\\s"

# define LF_RT_NAME "\\sname:\\s%s\\s"
# define LF_RT_MASK "\\seffect:\\s%s\\s"
# define LF_RT_ILLUM "\\sillum:\\s%s\\s"
# define LF_RT_TYPE "\\stype:\\s%s\\s"

# define LF_RT_MOVE		"\\smoving:\\s%w\\s%f\\s%f\\s"
# define LF_RT_ROTATE	"\\srotate:\\s%w\\s%f\\s%f\\s"

# define LF_RT_MTL			"\\smtl:\\sfiles/%w\\s"
# define LF_RT_AMBIANCE	"\\sambiance:\\s%w\\s%f\\s"
# define LF_RT_GATE_XOR "\\scsg:\\s%w\\s"
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
# define LF_MTL "\\smtllib\\s%w\\s"
# define LF_USEMTL "\\susemtl\\s%w\\s"

/*
	.MTL format defines
*/

# define LF_NEWMTL "\\snewmtl\\s%w\\s"
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
# define LF_MAP_KA "\\smap_Ka\\s%w\\s"
# define LF_MAP_KD "\\smap_Kd\\s%w\\s"
# define LF_MAP_REFL "\\smap_refl\\s%w\\s"

#define	FILE_CHECK_EXT(x, y) (ft_strrchr(x, '.') != NULL && !ft_strcmp(ft_strrchr(x, '.') + 1, y))

/*
	Other
*/

enum		e_primitive_type
{
	SPHERE,
	PLANE,
	CYLINDRE,
	CONE,
	CUBE,
	GLASS,
	DISK,
	CUBE_TROUE,
	CAMERA,
	POINT_LIGHT,
	SPOT_LIGHT,
	DIRECTIONAL_LIGHT,
};

enum					e_post_processing
{
	SEPIA =				0x0001,
	BLACK_AND_WHITE =	0x0002,
	CARTOON =			0x0004,
	ANTIALIASING =		0x0008,
	DEPTH_OF_FIELD =	0x0010,
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
	REFLECT =			0x008,
};

typedef	struct  s_primitive
{
	int					type;
	union {
		float			radius;
		float			height;
		float			angle;
	};
	t_vec4			slice[5];
	int					nsl;
}				t_primitive;

typedef struct			s_transform
{
	t_vec3		position;
	t_vec3		initial_position;
	t_vec3		euler_angles;
	t_vec3		normal;
}						t_transform;

typedef struct			s_image
{
	char			file[1024];
	unsigned char	*data;
	int				width;
	int				height;
	int				channels;
	t_vec4			atlas_uv;
}						t_image;

typedef	struct 			s_material
{
	char				name[256];
	int					illum;
	t_vec3				color;
	t_vec3				emission_color;
	t_vec3				highlight_color;
	float				opacity;
	float				specular;
	float				reflection;
	float				refraction;
	float				bump;
	t_image				texture;
	t_image				bumpmap;
	t_image				emission_map;
	t_image				highlight_map;
	t_image				reflection_map;
	t_image				refraction_map;
	t_image				opacity_map;
	t_image				specular_map;
	bool				has_texture;
	bool				has_bumpmap;
	bool				has_emission_map;
	bool				has_highlight_map;
	bool				has_reflection_map;
	bool				has_refraction_map;
	bool				has_opacity_map;
	bool				has_specular_map;
}						t_material;

typedef struct			s_submesh
{
	char			name[128];
	t_material		*material;
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
	t_vec3		scale;
}						t_mesh;

typedef struct			s_camera
{
	float		fov;
	float		ambient;
	int			post_processing_mask;
}						t_camera;

typedef struct			s_light
{
	t_vec3	color;
	float		intensity;
}						t_light;

typedef struct s_object	t_object;

typedef struct			s_object
{
	char		name[256];
	int			indent_level;
	t_transform	transform;
	t_light			light_prop;
	union {
		t_material		material;
		t_camera		camera;
	};
	union {
		t_mesh		mesh;
		t_primitive	primitive;
	};
	bool		is_primitive;
	t_vec4		move;
	t_vec4		rotate;
	float		laps;
	t_object	*children;
	t_object	*parent;
	t_object	*brother_of_children;
}						t_object;

typedef struct			s_scene
{
	int					nb_object;
	t_object			*root_view;
}						t_scene;

/*
GLSL uniform construction:
vec4 positon = vec3(position) + primitive spec value
vec4 rotaion = vec3(rotaion) + illum value

sampler2D textureAtlas;
sampler2D textureAtlasUvs;

...

*/

/*
	Parsing
*/

void					parse_obj(char *file, t_mesh **mesh);
void					parse_mtl(t_mesh *mesh);
int						ft_sscanf(char *format, char *str, ...);
int						gl(char *dst, int *fd);
void					parse_rt_file(char *file, t_scene *prim);
void					parse_obj_file(char *file, t_mesh *mesh);
int						get_next_word(char **str, char *res);
int						parse(int ac, char **av);

#endif
