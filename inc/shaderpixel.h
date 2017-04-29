/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaderpixel.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/07/11 18:11:58 by alelievr          #+#    #+#             */
/*   Updated  2016/07/25 19:04:51 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERPIXEL_H
# define SHADERPIXEL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# define GL_GLEXT_PROTOTYPES
# define GLFW_INCLUDE_GLCOREARB
# include "GLFW/glfw3.h"
# include "SOIL2.h"
# include "fmod.h"
# include "libft.h"
# undef PLUS

# if __APPLE__
#  define FMOD_LIB "fmod/lib/libfmod.dylib"
# else
#  define FMOD_LIB "fmod/lib/libfmod-linux.so.8.8"
# endif

# define WIN_W 1080
# define WIN_H 720
# define SCALE 70
//# define DOUBLE_PRECISION 1

typedef struct s_scene	t_scene;

typedef	struct			s_vec2
{
	float				x;
	float				y;
}						t_vec2;

typedef	struct 			s_vec3
{
	float				x;
	float				y;
	float				z;
}						t_vec3;

typedef	struct			s_vec4
{
	float				x;
	float				y;
	float				z;
	float				w;
}						t_vec4;

typedef struct	s_file
{
	char	path[1024];
	int		fd;
}				t_file;

typedef struct	s_line_list
{
	char				*line;
	struct s_line_list	*next;
}				t_line_list;

typedef struct	s_shader_file
{
	t_line_list		*begin;
	t_line_list		*function_begin;
	t_line_list		*uniform_begin;
	t_line_list		*main_image_begin;
	t_line_list		*raytrace_lights;
	t_line_list		*scene_begin;
	t_line_list		*scene_end;
}				t_shader_file;

typedef struct		s_atlas
{
	unsigned char	*data;
	GLuint			id;
	int				width;
	int				height;
}					t_atlas;

enum			KEY_BITS
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
	FORWARD,
	BACK,
	PLUS,
	MOIN,
	SOBJ_DIR_X,
	SOBJ_DIR_Y,
	SOBJ_DIR_Z,
	SOBJ_POS_RIGHT,
	SOBJ_POS_FORWARD,
	SOBJ_POS_BACK,
	SOBJ_POS_LEFT,
	SOBJ_POS_UP,
	SOBJ_POS_DOWN,
};

enum			SOUND_FORMAT
{
	WAVE,
};

typedef struct	s_sound
{
	int				id;
	int				fd;
	int				tex_length;
	GLint			gl_id;
	char			*image_buffer;
	FMOD_SOUND		*sound;
	enum SOUND_FORMAT	sound_format;
}				t_sound;

typedef struct		s_selected
{
	int		pos_uniform;
	int		dir_uniform;
	t_vec3	*dir;
	t_vec3	*pos;
}					t_selected;

#define BIT_SET(i, pos, v) (v) ? (i |= 1 << pos) : (i &= ~(1 << pos))
#define BIT_GET(i, pos) (i >> pos) & 1
#define g_move_AMOUNT 0.05f;

extern t_vec4		g_mouse;
extern t_vec4		g_move;
extern t_vec2		g_window;
extern t_vec3		g_forward;
extern int			g_shadow;
extern int			g_keys;
extern int			g_input_pause;
extern float		g_paused_time;
extern long			g_last_modified_file[0xF00];
extern float		g_fov;
extern float		g_ambient;
extern t_selected	g_selected_object;
extern int			g_selected_object_index;

GLFWwindow		*init(char *fname);
GLuint			create_program(char *file, bool fatal);
float			getCurrentTime(void);
bool			file_is_regular(int fd) __attribute__((overloadable));
bool			file_is_regular(char *fd) __attribute__((overloadable));
char			*build_shader(t_scene *scene, char *scene_directory, int *atlas_id, t_file *sources);
t_scene			*get_scene(t_scene *scene);
int				get_program(int p);
void			save_object_transform(void);
void			select_object(void);

static const char* vertex_shader_text =
"#version 330\n"
//"in vec2		iResolutionIn;\n"
//"out vec2		iResolution;\n"
"in vec2		fragPosition;\n"
"out vec4		outColor;\n"
"void main()\n"
"{\n"
//"	iResolution = iResolutionIn;\n"
"	gl_Position = vec4(fragPosition, 0.0, 1.0);\n"
"}\n";

static const char* fragment_shader_image_text =
"void mainImage(vec2 fragCoord)\n"
"{\n"
"	vec2 uv = fragCoord.xy / iResolution.xy;\n"
"	fragColor = vec4(uv, 0.5 + 0.5 * sin(iGlobalTime), 1.0);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec4 outColor;\n"
"out vec4 fragColor;\n"
"\n"
"uniform vec2		iResolution;\n"
"uniform float		iGlobalTime;\n"
"uniform int		iFrame;\n"
"uniform vec4		iMouse;\n"
"uniform vec3		iForward;\n"
"uniform vec4		iMoveAmount;\n"
"uniform sampler2D	atlas;\n"
"uniform float		iFov;"
"uniform float		iAmbient;"
"uniform int		iShadow = 0;\n"
"\n"
"void mainImage(vec2 f);\n"
"\n"
"vec4 texture2D(sampler2D s, vec2 coord, float f)\n"
"{\n"
"	return texture(s, coord, f);\n"
"}\n"
"\n"
"vec4 texture2D(sampler2D s, vec2 coord)\n"
"{\n"
"	return texture(s, coord);\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"	mainImage(gl_FragCoord.xy);\n"
"}\n";

static const char *main_image_start_text =
"#line 1\n"
"float pi = 3.1415;\n"
"void		progressbar(vec2 fragCoord)\n"
"{\n"
"	float radius = 0.3;\n"
"float lineWidth = 5.0; // in pixels\n"
"float glowSize = 5.0; // in pixels\n"
"\n"
"float pixelSize = 1.0 / min(iResolution.x, iResolution.y);\n"
"lineWidth *= pixelSize;\n"
"glowSize *= pixelSize;\n"
"glowSize *= 2.0;\n"
"\n"
"vec2 uv = (fragCoord.xy / iResolution.xy)-0.5;\n"
"uv.x *= iResolution.x/iResolution.y;\n"
"\n"
"float len = length(uv);\n"
"float angle = atan(uv.y, uv.x);\n"
"\n"
"float fallOff = fract(-0.5*(angle/pi)-iGlobalTime*0.5);\n"
"\n"
"lineWidth = (lineWidth-pixelSize)*0.5*fallOff;\n"
"float color = smoothstep(pixelSize, 0.0, abs(radius - len) - lineWidth)*fallOff;\n"
"color += smoothstep(glowSize*fallOff, 0.0, abs(radius - len) - lineWidth)*fallOff*0.5;    \n"
"\n"
"fragColor = vec4(color);\n"
"}\n"
"\n"
"void        mainImage(vec2 coord)\n"
"{\n"
"	vec2    uv = ((coord / iResolution) - .5) * 2.f;\n"
"	vec3    cameraPos = iMoveAmount.xyz;\n"
"	vec3    cameraDir = iForward;\n"
"	vec3    col;\n"
"\n"
"	//window ratio correciton:\n"
"	uv.x *= iResolution.x / iResolution.y;\n"
"	if (iGlobalTime < 3.f) { progressbar(coord); return ;}\n"
//"	fragColor = texture(atlas, uv);\n"
//"	return ;"
"\n"
"	//perspective view\n"
"	vec3    forw = normalize(iForward);\n"
"	vec3    right = normalize(cross(forw, vec3(0, 1, 0)));\n"
"	vec3    up = normalize(cross(right, forw));\n"
"	vec3    rd = normalize(uv.x * right + uv.y * up + iFov * forw);\n"
"	fragColor = vec4(raytrace(cameraPos, rd), 0);\n"
"}\n";

static const char *scene_start_text =
"Hit     scene(Ray r)\n"
"{\n"
"	int i = -1;\n"
"Hit hit = Hit(1e20,vec3(0,0,0),vec3(0,0,0),Material(vec4(0,0,0,0),vec4(0,0,0,0),vec4(0,0,0,0),vec4(0,0,0,0),vec4(0,0,0,0),vec4(0,0,0,0)),vec2(0,0),false);\n";

static const char *scene_end_text =
"    return hit;\n"
"}\n";

static const char *shader_header_text =
"struct      Ray\n"
"{\n"
"    vec3    dir;\n"
"    vec3    pos;\n"
"};\n"
"\n"
"struct  Coupes\n"
"{\n"
"    vec4		 t[5];\n"
"		 int		 nsl;\n"
"};\n"
"\n"
"struct  Material\n"
"{\n"
"	vec4		texture;\n"
"	vec4		emission;\n"
"	vec4		specular;\n"
"	vec4		reflection;\n"
"	vec4		refraction;\n"
"	vec4		bump;\n"
"};\n"
"\n"
"struct      Hit\n"
"{\n"
"   float  		dist;\n"
"   vec3    	norm;\n"
"   vec3    	pos;\n"
"	Material	mat;\n"
"	vec2		uv;\n"
"	bool		inside;\n"
"};\n"
"\n"
"Hit       scene(Ray r);\n"
"vec3    raytrace(vec3 ro, vec3 rd);\n";

static const char *raytrace_start_text =
"vec3    raytrace(vec3 ro, vec3 rd)\n"
"{\n"
"    Ray         r;\n"
"    Hit         h;\n"
"    vec3        color = vec3(0,0,0);\n"
"    vec3        ambient;\n"
"    r.dir = rd;\n"
"    r.pos = ro;\n"
"	// h = scene(r);\n"
"	//if(h.dist < 1e20)\n"
"	//	color = atlas_fetch(h.mat.texture, h.uv).xyz;\n";

static const char *raytrace_end_text =
"	return (color);\n"
"}\n";

#endif
