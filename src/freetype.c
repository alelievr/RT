/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/01/30 00:25:32 by alelievr         ###   ########.fr       */
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//load font's ascii table
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("error while loading char: %c (0x%x)\n", c, c);
			continue ;
		}

		GLuint	texture;
		glGenTextures(1, &texture);
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
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		font[c] = (t_font){texture, {face->glyph->bitmap.width, face->glyph->bitmap.rows}, {face->glyph->bitmap_left, face->glyph->bitmap_top}, face->glyph->advance.x};
	}
	//unbind opengl texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//free font
	FT_Done_Face(face);
	FT_Done_FreeType(g_library);

	//generate render buffers for fonts
	glGenVertexArrays(1, &g_vao);
	glGenBuffers(1, &g_vbo);

	//init render buffers
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//get texColor variable from shader for text color
	g_color_uniform = glGetUniformLocation(font_program, "textColor");
	GLfloat black[4] = {0, 0, 0, 1};
	glUniform4fv(g_color_uniform, 1, black);

	//unbind all
	glBindVertexArray(0);
	glUseProgram(0);
}

void	draw_text(const char *text, float x, float y)
{
	t_font			*font;

	glUseProgram(g_font_program);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(g_vao);
	float scale = 1;
	font = get_font_data(0);
	while (*text)
	{
		t_font c = font[(int)*text];

		GLfloat	xpos = x + c.bearing.x * scale;
		GLfloat	ypos = y - (c.size.y - c.bearing.y) * scale;

		GLfloat	w = c.size.x * scale;
		GLfloat	h = c.size.y * scale;

		GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

//		printf("xpos: %f - ypos: %f - w: %f - h: %f\n", xpos, ypos, w, h);

		//bind char texture
		glBindTexture(GL_TEXTURE_2D, c.textureID);

		//bind render zone
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		//unbind render zone
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (c.advance >> 6) * scale;

		text++;
  	}
	//unbind render infos
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
