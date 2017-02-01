/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/01/31 18:46:22 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <dirent.h>
#include <stdbool.h>

#define FILE_CHECK_EXT(x, y) (ft_strrchr(x, '.') != NULL && !ft_strcmp(ft_strrchr(x, '.') + 1, y))

static FT_Library	g_library;
static GLuint		g_vbo;
static GLuint		g_vao;
static GLuint		g_color_uniform;
static GLuint		g_font_program;
static GLuint		g_text_uniform;

const static float points[] = {
	-1.0f,  -1.0f,
	-1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f,
	-1.0f,  -1.0f,
};

static char	*foreach_font_file(void)
{
	struct dirent			*d;
	static DIR				*dir;
	static bool				init = false;
	static char				path[1024];

	if (!init && (init = true))
		dir = opendir("./fonts/");
	if (dir == NULL)
		return (NULL);
	while ((d = readdir(dir)))
	{
		if (FILE_CHECK_EXT(d->d_name, "ttf"))
		{
			ft_sprintf(path, "./fonts/%s", d->d_name);
			return (path);
		}
	}
	return (NULL);
}

typedef struct	s_vec2i
{
	int		x;
	int		y;
}				t_vec2i;

typedef struct	s_font
{
	GLuint	textureID;
	t_vec2i	size;
	t_vec2i	bearing;
	GLuint	advance;
}				t_font;

static t_font	*get_font_data(const int index)
{
	static t_font		fonts[0xF0][128];

	if (index >= 0 && index < 0xF0)
		return (fonts[index]);
	return (NULL);
}

void		load_fonts(GLuint font_program)
{
	int		error;
	char	*f;
	t_font	*font;
	FT_Face	face;
	int		index;

	//use and store font shader
	g_font_program = font_program;
	glUseProgram(font_program);

	//fonts loading
	index = 0;
	error = FT_Init_FreeType(&g_library);
	if (error)
		ft_printf("can't initialize freetype\n"), exit(-1);
	while ((f = foreach_font_file()))
	{
		if ((font = get_font_data(index++)) == NULL)
		{
			ft_printf("font limit reached, no more fonts will be loaded\n");
			break ;
		}
		if ((error = FT_New_Face(g_library, f, 0, &face)))
			ft_printf("error while loading font file: %f\n", f);
		FT_Set_Char_Size(face, 0, 48 * 64, window.x, window.y);
		FT_Set_Pixel_Sizes(face, 0, 48);
		ft_printf("%s font loaded\n", f);
	}

	//opengl options
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//TODO: create a temp buffer to flip bitmap

	//load font's ascii table
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("error while loading char: %c (0x%x)\n", c, c);
			continue ;
		}

		ALIAS(face->glyph->bitmap, bitmap);
		for (unsigned x = 0; x < bitmap.width; x++)
		{
			for (unsigned y = 0; y < bitmap.rows; y++)
			{
				//flip y
//				char top = bitmap.buffer[x * bitmap.width + y];
//				bitmap.buffer[x * bitmap.width + y] = bitmap.buffer[(bitmap.width - x - 1) * bitmap.width + y];
//				bitmap.buffer[(bitmap.width - x - 1) * bitmap.width + y] = top;
				printf("\033[48;5;%im ", face->glyph->bitmap.buffer[x * face->glyph->bitmap.width + y]);
			}
			printf("\n");
		}

		GLuint	texture;

		glGenTextures(1, &texture);
		int error = glGetError();
		if (error)
			printf("ERR: %i\n", error);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);
		if ((error = glGetError()))
			printf("ERR: %i\n", error);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		printf("OpenGL textue ID: %i\n", texture);
		font[c] = (t_font){texture, {face->glyph->bitmap.width, face->glyph->bitmap.rows}, {face->glyph->bitmap_left, face->glyph->bitmap_top}, face->glyph->advance.x};
	}
	//unbind opengl texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//free font
	FT_Done_Face(face);
	FT_Done_FreeType(g_library);

	//init render buffers
	glGenBuffers(1, &g_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, NULL, GL_DYNAMIC_DRAW);

	//init vao
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, NULL);
	glBindVertexArray(0);

	//get texColor variable from shader for text color
	g_color_uniform = glGetUniformLocation(font_program, "textColor");
	GLfloat black[4] = {0, 0, 0, 1};
	glUniform4fv(g_color_uniform, 1, black);

	g_text_uniform = glGetUniformLocation(font_program, "tex");

	//unbind all
	glBindVertexArray(0);
	glUseProgram(0);

	(void)points;
}

void	draw_text(const char *text, float x, float y)
{
	t_font			*font;

	glUseProgram(g_font_program);
	float scale = 10;
	font = get_font_data(0);
	while (*text)
	{
		t_font c = font[(int)*text];

		(void)y;
		GLfloat	xpos = x + c.bearing.x * scale;
		GLfloat	ypos = y - (c.size.y - c.bearing.y) * scale;

		GLfloat	w = c.size.x * scale;
		GLfloat	h = c.size.y * scale;

        GLfloat vertices[] = {
			xpos,     ypos,
			xpos,     ypos + h,
			xpos + w, ypos + h,

			xpos + w, ypos + h,
			xpos + w, ypos,
			xpos    , ypos
		};

		glBindBuffer (GL_ARRAY_BUFFER, g_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUniform2f(glGetUniformLocation(g_font_program, "iResolution"), window.x, window.y);

		//bind char texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, c.textureID);
		glUniform1i(g_text_uniform, 1);

		glBindVertexArray(g_vao);

        // Render quad
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

		x += (c.advance >> 6) * scale;

		text++;
		(void)vertices;
  	}
	//unbind render infos
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
