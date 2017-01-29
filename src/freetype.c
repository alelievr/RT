/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/01/29 03:43:22 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <dirent.h>
#include <stdbool.h>

#define FILE_CHECK_EXT(x, y) (ft_strrchr(x, '.') != NULL && !ft_strcmp(ft_strrchr(x, '.') + 1, y))
#define NONE (void *)(-1)

static FT_Library	g_library;
static GLuint		g_vbo;
static GLuint		g_vao;
static GLuint		g_tex;
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

static FT_Face	*get_face_data(const int index)
{
	static FT_Face		faces[0xF0];

	if (index >= 0 && index < 0xF0)
		return faces + index;
	return (NONE);
}

void		load_fonts(GLuint font_program)
{
	int		error;
	char	*f;
	FT_Face	*face;
	int		index;

	index = 0;
	error = FT_Init_FreeType(&g_library);
	if (error)
		ft_printf("can't initialize freetype\n"), exit(-1);
	while ((f = foreach_font_file()))
	{
		if ((face = get_face_data(index++)) == NONE)
		{
			ft_printf("font limit reached, no more fonts will be loaded\n");
			break ;
		}
		if ((error = FT_New_Face(g_library, f, 0, face)))
			ft_printf("error while loading font file: %f\n", f);
		FT_Set_Char_Size(*face, 0, 48 * 64, window.x, window.y);
		FT_Set_Pixel_Sizes(*face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		ft_printf("%s font loaded\n", f);
	}

	g_font_program = font_program;
	glUseProgram(font_program);

	glGenVertexArrays(1, &g_vao);
	glGenBuffers(1, &g_vbo);

	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

/*	GLuint      uniform_tex = glGetUniformLocation(font_program, "tex");
	glGenTextures(1, &g_tex);
	glBindTexture(GL_TEXTURE_2D, g_tex);
	glUniform1i(uniform_tex, 0);*/

	g_color_uniform = glGetUniformLocation(font_program, "textColor");
	GLfloat black[4] = {0, 0, 0, 1};
	glUniform4fv(g_color_uniform, 1, black);

	glBindVertexArray(0);
	glUseProgram(0);
}

void	draw_text(const char *text, float x, float y, float sx, float sy)
{
  	const char		*p;
	FT_Face			*face;
	int				error;

	printf("draw text: %s\n", text);
	glUseProgram(g_font_program);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(g_vao);
	float scale = 1;
	face = get_face_data(0);
  	for (p = text; *p; p++)
	{
		FT_UInt  glyph_index;
		glyph_index = FT_Get_Char_Index(*face, *p);
		error = FT_Load_Glyph(*face, glyph_index, FT_LOAD_DEFAULT);
		FT_GlyphSlot g = (*face)->glyph;
		if (error)
			continue ;

    	glTexImage2D(
      			GL_TEXTURE_2D,
      			0,
      			GL_RED,
      			g->bitmap.width,
      			g->bitmap.rows,
      			0,
      			GL_RED,
      			GL_UNSIGNED_BYTE,
      			g->bitmap.buffer
    			);

 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

        GLfloat xpos = x + g->bitmap_left * scale;
        GLfloat ypos = y - (g->bitmap.rows - g->bitmap_top) * scale;

        GLfloat w = g->bitmap.rows * scale;
        GLfloat h = g->bitmap.width * scale;
		
		GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

		glBindTexture(GL_TEXTURE_2D, g_tex);
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);

    	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    	x += (g->advance.x/64) * sx;
    	y += (g->advance.y/64) * sy;
  	}
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
