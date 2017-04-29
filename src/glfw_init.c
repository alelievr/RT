/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glfw_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created  2016/07/21 00:12:11 by alelievr          #+#    #+#             */
/*   Updated  2016/07/21 17:56:22 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaderpixel.h"
#include <math.h>

static t_vec2	angleAmount;
static int	cursor_mode;
static float lastPausedTime;

static void		error_callback(int error, const char *description)
{
	(void)error;
	fprintf(stderr, "Error: %s\n", description);
}

static void		key_callback(GLFWwindow *g_window, int key,int scancode, int action, int mods)
{
	(void) scancode;
	(void) mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(g_window, GLFW_TRUE);
	if (key == GLFW_KEY_RIGHT)
		BIT_SET(g_keys, RIGHT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_LEFT)
		BIT_SET(g_keys, LEFT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_UP)
		BIT_SET(g_keys, FORWARD, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_DOWN)
		BIT_SET(g_keys, BACK, action == GLFW_PRESS || action == GLFW_REPEAT);

	if (key == GLFW_KEY_D)
		BIT_SET(g_keys, SOBJ_POS_RIGHT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_A)
		BIT_SET(g_keys, SOBJ_POS_LEFT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_W)
		BIT_SET(g_keys, SOBJ_POS_FORWARD, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_S)
		BIT_SET(g_keys, SOBJ_POS_BACK, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_Q)
		BIT_SET(g_keys, SOBJ_POS_UP, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_E)
		BIT_SET(g_keys, SOBJ_POS_DOWN, action == GLFW_PRESS || action == GLFW_REPEAT);

	if (key == GLFW_KEY_X)
		BIT_SET(g_keys, SOBJ_DIR_X, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_Y)
		BIT_SET(g_keys, SOBJ_DIR_Y, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_Z)
		BIT_SET(g_keys, SOBJ_DIR_Z, action == GLFW_PRESS || action == GLFW_REPEAT);

//	if (key == GLFW_KEY_Q)
//		BIT_SET(g_keys, UP, action == GLFW_PRESS || action == GLFW_REPEAT);
//	if (key == GLFW_KEY_E)
//		BIT_SET(g_keys, DOWN, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL)
		BIT_SET(g_keys, PLUS, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS)
		BIT_SET(g_keys, MOIN, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_TAB && !(mods & GLFW_MOD_SHIFT) && action == GLFW_PRESS)
		g_selected_object_index++, select_object();
	if (key == GLFW_KEY_TAB && (mods & GLFW_MOD_SHIFT) && action == GLFW_PRESS)
		g_selected_object_index--, select_object();
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		g_shadow ^= action;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		g_input_pause ^= action;
		if (g_input_pause)
			lastPausedTime = getCurrentTime();
		else
			g_paused_time += getCurrentTime() - lastPausedTime;
	}
	if (key == GLFW_KEY_C)
		cursor_mode ^= action == GLFW_PRESS;
	glfwSetInputMode(g_window, GLFW_CURSOR, (cursor_mode) ? GLFW_CURSOR_NORMAL \
	: GLFW_CURSOR_DISABLED);
}

static void		g_mouse_callback(GLFWwindow *win, double x, double y)
{
	(void)win;
	g_mouse.x = x;
	g_mouse.y = y;
	if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		angleAmount.x += ((g_window.x / 2.) - g_mouse.x) / 200;
		angleAmount.y += ((g_window.y / 2.) - g_mouse.y) / 200;
		if (angleAmount.y > 1.5f)
			angleAmount.y = 1.5f;
		if (angleAmount.y < -1.5f)
			angleAmount.y = -1.5f;
		g_forward.x = cos(angleAmount.y) * sin(angleAmount.x);
		g_forward.y = sin(angleAmount.y);
		g_forward.z = cos(angleAmount.y) * cos(angleAmount.x);
	}
}

static void		g_mouse_click_callback(GLFWwindow *win, int button, \
	int action, int mods)
{
	(void)win;
	(void)action;
	(void)mods;
	(void)button;
	if (action == 1)
		g_mouse.z = 1;
	else
		g_mouse.z = 0;
}

static void		resize_callback(GLFWwindow *win, int width, int height)
{
	(void)win;
	g_window.x = width;
	g_window.y = height;
}

GLFWwindow		*init(char *name)
{
	GLFWwindow *win;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		printf("glfwInit error !\n");
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!(win = glfwCreateWindow(WIN_W, WIN_H, name, NULL, NULL)))
	{
		printf("failed to create g_window !\n");
		exit(-1);
	}
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, g_mouse_callback);
	glfwSetMouseButtonCallback(win, g_mouse_click_callback);
	glfwSetWindowSizeCallback(win, resize_callback);
	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return (win);
}
