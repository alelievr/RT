/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freetype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 17:03:04 by alelievr          #+#    #+#             */
/*   Updated: 2017/01/28 02:42:52 by alelievr         ###   ########.fr       */
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
static GLuint		g_tex;

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
		FT_Set_Char_Size(*face, 0, 16 * 64, window.x, window.y);
		FT_Set_Pixel_Sizes(*face, 0, 16);
		ft_printf("%s font loaded\n", f);
	}

	glGenBuffers(1, &g_vbo);
	GLint attribute_coord = glGetAttribLocation(font_program, "coord");
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint      uniform_tex = glGetUniformLocation(font_program, "tex");
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &g_tex);
	glBindTexture(GL_TEXTURE_2D, g_tex);
	glUniform1i(uniform_tex, 0);
}

/*void		draw_text(int x, int y, const int fontIndex, const char *txt)
{
	FT_Face			*face;
	FT_GlyphSlot	slot;
	int				error;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	face = get_face_data(fontIndex);
	if (face == NONE || !*face)
		ft_printf("font not found at intdex: %i\n", fontIndex), exit(-1);
	slot = (*face)->glyph;
	while (*txt)
	{
		FT_UInt  glyph_index;
		glyph_index = FT_Get_Char_Index(*face, *txt);
		error = FT_Load_Glyph(*face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue ;
		error = FT_Render_Glyph((*face)->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			continue;

  		my_draw_bitmap(&slot->bitmap,
                x + slot->bitmap_left,
                y - slot->bitmap_top,
				slot,
				16);

		x += slot->advance.x >> 6;
		y += slot->advance.y >> 6;

		txt++;
	}
}*/
void	draw_text(const char *text, float x, float y, float sx, float sy)
{
  	const char		*p;
	FT_Face			*face;
	int				error;

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

    	float x2 = x + g->bitmap_left * sx;
    	float y2 = -y - g->bitmap_top * sy;
    	float w = g->bitmap.width * sx;
    	float h = g->bitmap.rows * sy;

    	GLfloat box[4][4] = {
        	{x2,     -y2    , 0, 0},
        	{x2 + w, -y2    , 1, 0},
        	{x2,     -y2 - h, 0, 1},
        	{x2 + w, -y2 - h, 1, 1},
    	};

    	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    	x += (g->advance.x/64) * sx;
    	y += (g->advance.y/64) * sy;
  	}
}
