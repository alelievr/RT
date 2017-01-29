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

static vec2	angleAmount;
static int	cursor_mode;
static float last_paused_time;

static void error_callback(int error, const char* description)
{
	(void)error;
	fprintf(stderr, "Error: %s\n", description);
}

static void	focus_callback(GLFWwindow *win, int focused)
{
	(void)win;
	focus = focused;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
		BIT_SET(keys, RIGHT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
		BIT_SET(keys, LEFT, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
		BIT_SET(keys, FORWARD, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
		BIT_SET(keys, BACK, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_Q)
		BIT_SET(keys, UP, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_E)
		BIT_SET(keys, DOWN, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL)
		BIT_SET(keys, PLUS, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS)
		BIT_SET(keys, MOIN, action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_SPACE)
	{
		input_pause ^= action;
		if (input_pause)
			last_paused_time = get_current_time();
		else
			paused_time += get_current_time() - last_paused_time;
	}
	if (key == GLFW_KEY_C)
		cursor_mode ^= action == GLFW_PRESS;
	glfwSetInputMode(window, GLFW_CURSOR, (cursor_mode) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

static void mouse_callback(GLFWwindow *win, double x, double y)
{
	(void)win;
	mouse.x = x;
	mouse.y = y;

	if (glfwGetInputMode(win, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		angleAmount.x += ((window.x / 2.) - mouse.x) / 200;
		angleAmount.y += ((window.y / 2.) - mouse.y) / 200;

		if (angleAmount.y > 1.5f)
			angleAmount.y = 1.5f;
		if (angleAmount.y < -1.5f)
			angleAmount.y = -1.5f;
		forward.x = cos(angleAmount.y) * sin(angleAmount.x);
		forward.y = sin(angleAmount.y);
		forward.z = cos(angleAmount.y) * cos(angleAmount.x);
	}
}

static void mouse_click_callback(GLFWwindow *win, int button, int action, int mods)
{
	(void)win;
	(void)action;
	(void)mods;
	(void)button;
	if (action == 1)
		mouse.z = 1;
	else
		mouse.z = 0;
}

static void resize_callback(GLFWwindow *win, int width, int height)
{
	(void)win;
	window.x = width;
	window.y = height;
}

GLFWwindow	*init(char *name)
{
	GLFWwindow *win;

	//GLFW
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		printf("glfwInit error !\n"), exit(-1);
//	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!(win = glfwCreateWindow(WIN_W, WIN_H, name, NULL, NULL)))
		printf("failed to create window !\n"), exit(-1);
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_click_callback);
	glfwSetWindowSizeCallback(win, resize_callback);
	glfwSetWindowFocusCallback(win, focus_callback);
	glfwMakeContextCurrent (win);
	glfwSwapInterval(1);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return (win);
}
