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
GLuint			create_program(t_file *fd, size_t num, bool fatal);
float			getCurrentTime(void);

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
"uniform sampler2D	iChannel0;\n"
"uniform sampler2D	iChannel1;\n"
"uniform sampler2D	iChannel2;\n"
"uniform sampler2D	iChannel3;\n"
"uniform sampler2D	iChannel4;\n"
"uniform sampler2D	iChannel5;\n"
"uniform sampler2D	iChannel6;\n"
"uniform sampler2D	iChannel7;\n"
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

#endif
