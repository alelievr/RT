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
# include "parser.h"
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

typedef struct s_vec22
{
	float x;
	float y;
}				vec2;

typedef struct s_vec33
{
	float x;
	float y;
	float z;
}				vec3;

typedef struct s_vec44
{
	float x;
	float y;
	float z;
	float w;
}				vec4;

typedef struct s_dvec4
{
	double x;
	double y;
	double z;
	double w;
}				dvec4;

typedef struct	s_file
{
	char	path[1024];
	int		fd;
}				t_file;

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

#define BIT_SET(i, pos, v) (v) ? (i |= 1 << pos) : (i &= ~(1 << pos))
#define BIT_GET(i, pos) (i >> pos) & 1
#define MOVE_AMOUNT 0.01f;

extern vec4			mouse;
extern vec4			move;
extern vec2			window;
extern vec3			forward;
extern int			keys;
extern int			input_pause;
extern float		pausedTime;
extern long			lastModifiedFile[0xF00];

GLFWwindow		*init(char *fname);
GLuint			create_program(char *file, bool fatal);
float			getCurrentTime(void);
bool			file_is_regular(int fd) __attribute__((overloadable));
bool			file_is_regular(char *fd) __attribute__((overloadable));

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
"}\n"
"#line 0\n";

static const char *main_image_start_text = 
"void        mainImage(vec2 coord)\n"
"{\n"
"	vec2    uv = (coord / iResolution) * 2 - 1;\n"
"	vec3    cameraPos = iMoveAmount.xyz + vec3(0, 0, -2);\n"
"	vec3    cameraDir = iForward;\n"
"	vec3    col;\n"
"\n"
"	//window ratio correciton:\n"
"	uv.x *= iResolution.x / iResolution.y;\n"
"\n"
"	//perspective view\n"
"	float   fov = 1.5;\n"
"	vec3    forw = normalize(iForward);\n"
"	vec3    right = normalize(cross(forw, vec3(0, 1, 0)));\n"
"	vec3    up = normalize(cross(right, forw));\n"
"	vec3    rd = normalize(uv.x * right + uv.y * up + fov * forw);\n"
"	fragColor = vec4(raytrace(cameraPos, rd), 1);\n"
"}\n";

static const char *scene_start_text =
"Hit     scene(Ray r)\n"
"{\n"
"	int i = -1;\n"
"	Hit     hit;\n"
"	hit.dist = 1e20;\n"
"	hit.mat.texture = vec4(0,0,0,0);\n";

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
"struct  Coupe\n"
"{\n"
"    vec3    pos;\n"
"    vec3    rot;\n"
"};\n"
"\n"
"struct  Material\n"
"{\n"
"	vec4		texture;\n"
"	vec4		emission;\n"
"	vec4		transparency;\n"
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
"    h = scene(r);\n";

static const char *raytrace_end_text =
"	return (color / 2);\n"
"}\n";

#endif
