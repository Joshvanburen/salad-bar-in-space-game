/*
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 */

#include <stdlib.h>
#include <math.h>

#ifndef WIN32
	#include <unistd.h>
	#define WIIUSE_PATH		"./wiiuse.so"
#else
	#define WIIUSE_PATH		"wiiuse.dll"

	#include <windows.h>

	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "sdl.lib")
	#pragma comment(lib, "sdlmain.lib")
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "wiiuse.h"

#define PI 3.14159265358979323846
#define PI_DIV_180 0.017453292519943296
#define deg PI_DIV_180

#define WIIMOTE_ID_1		1
#define WIIMOTE_ID_2		2

GLint width = 1024, height = 768;
GLfloat backColor[4] = {1.0,1.0,1.0,1.0};

wiimote** wiimotes = NULL;
float xcoord = 0;
float ycoord = 0;

int last_dots[4][2] = {{0}};

void handle_event(struct wiimote_t* wm);
void display();
void resize_window(GLint new_width, GLint new_height);

void handle_event(struct wiimote_t* wm) {
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS))
		wiiuse_motion_sensing(wm, 1);
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS))
		wiiuse_motion_sensing(wm, 0);

	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP))
		wiiuse_set_ir(wm, 1);
	if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN))
		wiiuse_set_ir(wm, 0);

	if (WIIUSE_USING_IR(wm)) {
		if (wm->ir.num_dots) {
			xcoord = wm->ir.x;
			ycoord = wm->ir.y;
		}
	}

	display();
}

#define DRAW_TRIANGLE(x, y, z, s)	do {							\
										glVertex3f(x, y-s, z);		\
										glVertex3f(x+s, y+s, z);	\
										glVertex3f(x-s, y+s, z);	\
									} while (0)

void display() {
	int i;
	float size = 5;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
		/* green center */
		glColor3f(0.0, 1.0, 0.0);
		DRAW_TRIANGLE(width/2, height/2, 0, size);

		/* red ir */
		glColor3f(1.0, 0.0, 0.0);
		for (i = 0; i < 4; ++i) {
			if (wiimotes[0]->ir.dot[i].visible)
				DRAW_TRIANGLE(wiimotes[0]->ir.dot[i].rx, wiimotes[0]->ir.dot[i].ry, 0, size);
		}

		/* yellow corrected ir */
		glColor3f(1.0, 1.0, 0.0);
		for (i = 0; i < 4; ++i) {
			if (wiimotes[0]->ir.dot[i].visible)
				DRAW_TRIANGLE(wiimotes[0]->ir.dot[i].x, wiimotes[0]->ir.dot[i].y, 0, size);
		}

		/* blue cursor */
		glColor3f(0.0, 0.0, 1.0);
		DRAW_TRIANGLE(wiimotes[0]->ir.x, wiimotes[0]->ir.y-size, 0, size);
	glEnd();

	SDL_GL_SwapBuffers();
}

void resize_window(GLint new_width, GLint new_height) {
	width = new_width;
	height = new_height;

	if (new_height == 0)
		new_height = 1;

	SDL_SetVideoMode(width, height, 16, SDL_RESIZABLE | SDL_OPENGL);

	glViewport(0, 0, new_width, new_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	width = new_width;
	height = new_height;

	wiiuse_set_ir_vres(wiimotes[0], width, height);
}

#ifndef WIN32
int main(int argc, char** argv) {
#else
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif
	const char* version;
	int ids[] = { WIIMOTE_ID_1, WIIMOTE_ID_2 };
	int found, connected;

	version = wiiuse_startup(WIIUSE_PATH);
	printf("Wiiuse Version = %s\n", version);
	if (!version) {
		fprintf(stderr, "Failed to load wiiuse library.\n");
		return 0;
	}
	wiimotes =  wiiuse_init(2, ids, handle_event, NULL, NULL);
	found = wiiuse_find(wiimotes, 2, 5);
	if (!found)
		return 0;
	connected = wiiuse_connect(wiimotes, 2);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1 | WIIMOTE_LED_4);
	wiiuse_rumble(wiimotes[0], 1);

	#ifndef WIN32
		usleep(200000);
	#else
		Sleep(200);
	#endif

	wiiuse_rumble(wiimotes[0], 0);
	wiiuse_motion_sensing(wiimotes[0], 1);
	wiiuse_set_ir(wiimotes[0], 1);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	/* set window size */
	width = wiimotes[0]->ir.vres[0];
	height = wiimotes[0]->ir.vres[1];
	SDL_SetVideoMode(width, height, 16, SDL_RESIZABLE | SDL_OPENGL);
	wiiuse_set_ir_vres(wiimotes[0], width, height);

	/* set OpenGL stuff */
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0, 0, 0, 0);

	/* set the size of the window */
	resize_window(width, height);

	while (1) {
		SDL_Event event;
		wiiuse_poll(wiimotes, 2);

		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_VIDEORESIZE:
				{
					/* resize the window */
					resize_window(event.resize.w, event.resize.h);
					break;
				}
				case SDL_QUIT:
				{
					/* shutdown */
					SDL_Quit();
					wiiuse_shutdown();
					return 0;
				}
				default:
				{
				}
			}
		}
	}
}
