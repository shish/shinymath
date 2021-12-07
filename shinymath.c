/*
 * main.c (c) Shish 2005
 * 
 * ShinyMath; a program used to teach myself OpenGL, and to demonstrate
 * how demo coders use math to make pretty shapes
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __WIN32
  #include <windows.h>
  #define LOADLIB(path)      LoadLibrary(path)
  #define GETFUNC(lib, func) GetProcAddress(lib, func)
  #define FREELIB(lib)       FreeLibrary(lib)
#else
  #include <dlfcn.h>
  #define LOADLIB(path)      dlopen(path, RTLD_NOW)
  #define GETFUNC(lib, func) dlsym(lib, func)
  #define FREELIB(lib)       dlclose(lib)
#endif

#include "shinymath.h"


#define WIDTH 640
#define HEIGHT 480


int time = 0;
int meshNum = 0;

GLfloat zoom = -10;
GLfloat yaw = 0;
GLfloat pitch = 0;

int grid;
mesh_t *mesh;

int initSDL();
int initOpenGL();
int initGrid();
int initMesh();

int doMesh();
int doRender();
int doFPS();
int doInput();


/*
 * start, main loop, end
 */
int main(int argc, char *argv[]) {
	if(initSDL() && initOpenGL() && initGrid() && initMesh())
		while(doMesh() && doRender() && doFPS() && doInput());

	SDL_Quit();
	
	return 1;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                            *
*                          Parts of the main loop                            *
*                                                                            *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * calculate average FPS over the last 8 frames
 */
int doFPS() {
	static int start = 0;
	static int now = 0;
	static int delta = 0;
	static int last[8];
	static int frame = 0;
	
	char title[128];
	int ave = 1;
	
	now = SDL_GetTicks();
	delta = now-start;
	start = SDL_GetTicks();
	
	last[frame++%8] = delta;
	
	ave = (last[0]+last[1]+last[2]+last[3]+
	       last[4]+last[5]+last[6]+last[7])/8;

	if(ave) {
		sprintf(title, "ShinyMath 0.2 %ifps", 1000/ave);
		SDL_WM_SetCaption(title, NULL);
	}

	// anti-aliasing off and no fps cap = 200FPS on slow hardware~
	if(1000/30-ave > 0) SDL_Delay(1000/30-ave);
	
	return 1;
}

static int isLibrary(char *fname) {
#ifdef __WIN32
	return (strcmp(fname+strlen(fname)-4, ".dll") == 0);
#else
	return (strcmp(fname+strlen(fname)-3, ".so") == 0);
#endif
}

/*
 * (re)generate the mesh, if it's changed since the last frame
 */
int doMesh() {
	static int lastMesh = -1;
	static int lastTime = -1;

	static void *lib;
	static mesh_t *(*getMesh)(int time);

	if(meshNum == lastMesh && time == lastTime) {
		// nothing has changed
	}
	else if(meshNum == lastMesh) {
		// only time has changed; don't bother relinking the library,
		// just reuse the static function pointer we found last time
		free(mesh->vertices);
		free(mesh);
		mesh = (*getMesh)(time);
		lastTime = time;
	}
	else {
		DIR *dp;
		struct dirent *entry;
		char meshLib[256], meshPath[256];
	
		if(meshNum < 0) meshNum = 0;

		dp = opendir("./gens/");
		if(!dp) {printf("error reading ./gens/\n"); return 1;}

		int currentLibNum = 0;
		while(entry = readdir(dp)) {
			if(isLibrary(entry->d_name)) {
				if(currentLibNum == meshNum) {
					strncpy(meshLib, entry->d_name, 128);
					currentLibNum = 0;
					break;
				}
				else {
					currentLibNum++;
				}
			}
		}
		
		closedir(dp);
		
		if(currentLibNum) {
			meshNum = currentLibNum-1;
			return 1;
		}
	
		lastMesh = meshNum;
		lastTime = time;

		sprintf(meshPath, "./gens/%s", meshLib);

		if(lib) FREELIB(lib);

		printf("Loading %s... ", meshPath);
		lib = LOADLIB(meshPath);
		if(!lib) {printf("error loading DLL\n"); return 1;}

		getMesh = (void *)GETFUNC(lib, "getMesh");
		if(!getMesh) {printf("error finding function\n"); return 1;}

		free(mesh->vertices);
		free(mesh);
		mesh = (*getMesh)(time);
		if(!mesh) {printf("error calling getMesh\n"); return 1;}

		printf("done\n");
	}

	return 1;
}

/*
 * get OGL to draw the grid and the mesh
 */
int doRender() {
	if(zoom < -30) zoom = -30;
	else if(zoom > -1) zoom = -1;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw,   0.0f, 1.0f, 0.0f);

    glCallList(grid);

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
	for(int i=0; i<mesh->pcount; i++) {
		if(mesh->vertices[i].draw) {
			glVertex3f(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
		}
		else {
		    glEnd();
		    glBegin(GL_LINE_STRIP);
		}
	}
    glEnd();

    SDL_GL_SwapBuffers();
    
    return 1;
}
		
/*
 * handle SDL input
 */
int doInput() {
	static int animate = 0;
    SDL_Event event;

	if(animate) {
	    SDL_PollEvent(&event);
	    time++;
	}
	else {
		SDL_WaitEvent(&event);
	}

    switch(event.type) {
	    case SDL_QUIT:
			return 0;
			break;
	    case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button) {
				case SDL_BUTTON_WHEELUP:   zoom += 2; break;
				case SDL_BUTTON_WHEELDOWN: zoom -= 2; break;
			}
			break;
	    case SDL_MOUSEMOTION:
			if(event.motion.state & SDL_BUTTON(1)) {
				yaw += event.motion.xrel;
				pitch += event.motion.yrel;
			}
			else if(event.motion.state & (SDL_BUTTON(2)|SDL_BUTTON(3))) {
				zoom -= (float)event.motion.yrel/4.0f;
			}
			break;
	    case SDL_KEYDOWN:
	        switch (event.key.keysym.sym) {
				case SDLK_q: case SDLK_ESCAPE: return 0; break;
				case SDLK_w: case SDLK_KP8: case SDLK_UP:    pitch -= 5; break;
				case SDLK_s: case SDLK_KP2: case SDLK_DOWN:  pitch += 5; break;
				case SDLK_a: case SDLK_KP4: case SDLK_LEFT:  yaw += 5;   break;
				case SDLK_d: case SDLK_KP6: case SDLK_RIGHT: yaw -= 5;   break;
				case SDLK_z: case SDLK_KP_PLUS:  zoom += 0.5; break;
				case SDLK_x: case SDLK_KP_MINUS: zoom -= 0.5; break;
				case SDLK_r: case SDLK_KP_ENTER: pitch=0; yaw=0; zoom=-10; break;
				case SDLK_n: case SDLK_KP_MULTIPLY: meshNum++; break;
				case SDLK_p: case SDLK_KP_DIVIDE:   meshNum--; break;
				case SDLK_f: case SDLK_KP7:         time++; break;
				case SDLK_b: case SDLK_KP1:         time--; break;
				case SDLK_e: case SDLK_KP5:         animate = !animate; break;
				default: break;
			}
			break;
	}
	
	return 1;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                            *
*                       Setup / Initialisation Stuff                         *
*                                                                            *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Boilerplate SDL+OGL init
 */
int initSDL() {
    const SDL_VideoInfo* video;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 0;
    }
    atexit(SDL_Quit);

    video = SDL_GetVideoInfo();
    if(!video) {
        fprintf(stderr, "Couldn't get video information: %s\n", SDL_GetError());
        return 0;
    }

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(SDL_SetVideoMode(WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL) == 0) {
        fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
        return 0;
    }
    SDL_WM_SetCaption("ShinyMath Loading...", NULL);

	return 1;
}

/*
 * Boilerplate OGL init
 */
int initOpenGL() {
    float aspect = (float)WIDTH / (float)HEIGHT;
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH); // antialiasing
    glDisable(GL_CULL_FACE); // remove faces that don't face the camera
	return 1;
}

/*
 * Make the grid into a display list
 */
int initGrid() {
	grid = glGenLists(1);
	if(!grid) return 0;

	glNewList(grid,GL_COMPILE);
	glBegin(GL_LINES);

	// x = red
	for(int y=0; y<21; y++) {
		if(y == 10) glColor3f(1.0f, 0.0f, 0.0f);
		else glColor3f(0.75f, 0.75f, 0.75f);

		glVertex3f(-10, -10+y, 0);
		glVertex3f(+10, -10+y, 0);
	}
	
	// y = green
	for(int x=0; x<21; x++) {
		if(x == 10) glColor3f(0.0f, 1.0f, 0.0f);
		else glColor3f(0.75f, 0.75f, 0.75f);

		glVertex3f(-10+x, -10, 0);
		glVertex3f(-10+x, +10, 0);
	}
	
	// z = blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, -10);
	glVertex3f(0, 0, +10);
	
	glEnd();
	glEndList();

	return 1;
}

/*
 * make a blank mesh
 */
int initMesh() {
	mesh = malloc(sizeof(mesh_t));
	mesh->vertices = NULL;
	mesh->pcount = 0;

	return (mesh != NULL);
}

