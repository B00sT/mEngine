#include "mEngine.h"

mEngine::mEngine()
{
	gfCheck = false;
}

mEngine::~mEngine()
{

}

void mEngine::set2D()
{
	if (gfCheck)
		return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
}

void mEngine::set3D()
{
	if (gfCheck)
		return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width/(float)height, 1.0, 1024.0);
	glMatrixMode(GL_MODELVIEW);
}

void mEngine::quit_prog(int code)
{
	SDL_Quit();
	exit(code);
}

// -- mX - mouse X position, mY - mouse Y position
int mEngine::checkMouseCollide(int mX, int mY)
{
	gfCheck = true;
	GLint lNumF = 0, lVPC[4] = {0};
	GLuint lSBuf[64] = {0};
	int id;

	glSelectBuffer(64, lSBuf);
	glGetIntegerv(GL_VIEWPORT, lVPC);
	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		gluPickMatrix(mX, height-mY, 1, 1, lVPC);
		gluPerspective(60.0, (float)width/(float)height, 1, 1024.0);

		glMatrixMode(GL_MODELVIEW);
		draw_screen_func();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	lNumF = glRenderMode(GL_RENDER);

	glMatrixMode(GL_MODELVIEW);

	gfCheck = false;

	if (lNumF > 0)
		return lSBuf[((lNumF-1)*4)+3];
	return 0;
}

void mEngine::process_events(void)
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				handle_key_down_func(&event.key.keysym);
				break;
			case SDL_KEYUP:
				handle_key_up_func(&event.key.keysym);
				break;
			case SDL_MOUSEMOTION:
				handle_mouse_motion_func(&event.motion);
				break;
			case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
				handle_mouse_button_func(&event.button);
				break;
			case SDL_QUIT:
				quit_prog(0);
				break;
		}
	}
}

void mEngine::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glInitNames();
}

void mEngine::mBegin(int name)
{
	glPushMatrix();
	glLoadIdentity();
	glPushName(name);
}

void mEngine::mEnd()
{
	glPopMatrix();
	glPopName();
}

void mEngine::setHandlers(	void(*handle_key_down_func_)(SDL_keysym *keysym),
				void(*handle_key_up_func_)(SDL_keysym *keysym),
				void(*handle_mouse_motion_func_)(SDL_MouseMotionEvent *lEvent),
				void(*handle_mouse_button_func_)(SDL_MouseButtonEvent *lEvent))
{
	handle_key_down_func		=	handle_key_down_func_;
	handle_key_up_func			=	handle_key_up_func_;
	handle_mouse_motion_func	=	handle_mouse_motion_func_;
	handle_mouse_button_func	=	handle_mouse_button_func_;
}

void mEngine::mInit(int width_, int height_, const char *font_fname, void(*draw_screen_func_)(void))
{
	const SDL_VideoInfo* info = NULL;
	int bpp = 0;
	int flags = 0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		quit_prog(1);
	}

	info = SDL_GetVideoInfo();

	if(!info) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
	        quit_prog(1);
	}

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	flags = SDL_OPENGL | SDL_FULLSCREEN;

	width = width_;
	height = height_;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		quit_prog(1);
	}
	
	srand(time(0));
	draw_screen_func = draw_screen_func_;
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, width, height);
	set3D();
	Font = new MFont(font_fname, 1.0f, -0.2f, 40, 0);
}
