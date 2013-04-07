#include "mEngine.h"

#include <stdio.h>
#include <stdlib.h>

#define cBox(ld2, wd2, hd2) (float[][3]){{-ld2, -hd2,  wd2},{-ld2,  hd2,  wd2},{ ld2,  hd2,  wd2},{ ld2, -hd2,  wd2},{-ld2, -hd2, -wd2},{-ld2,  hd2, -wd2},{ ld2,  hd2, -wd2},{ ld2, -hd2, -wd2},{-ld2,  hd2, -wd2},{-ld2,  hd2,  wd2},{-ld2, -hd2,  wd2},{-ld2, -hd2, -wd2},{ ld2,  hd2, -wd2},{ ld2,  hd2,  wd2},{ ld2, -hd2,  wd2},{ ld2, -hd2, -wd2},{-ld2,  hd2, -wd2},{-ld2,  hd2,  wd2},{ ld2,  hd2,  wd2},{ ld2,  hd2, -wd2},{-ld2, -hd2, -wd2},{-ld2, -hd2,  wd2},{ ld2, -hd2,  wd2},{ ld2, -hd2, -wd2}}

#define ARROW_UP	0
#define ARROW_DOWN	1

#define MOUSE_1		0
#define MOUSE_2		1
#define MOUSE_3		2

#define MOUSE_X		0
#define MOUSE_Y		1
 
const int width		= 800;
const int height	= 450;

static GLubyte red[]    = {255,   0,   0, 255};
static GLubyte green[]  = {  0, 255,   0, 255};
static GLubyte blue[]   = {  0,   0, 255, 255};
static GLubyte white[]  = {255, 255, 255, 255};
static GLubyte yellow[] = {255, 255,   0, 255};
static GLubyte black[]  = {  0,   0,   0, 255};
static GLubyte orange[] = {255, 128,   0, 255};
static GLubyte purple[] = {255,   0, 255, 255};

static bool gEsc = false;
static bool gEnter = false;
static bool gArrow[2];

static bool gMouseButton[3];
static int gMousePos[2];

static int gcheckMouseCollide_ret;
static mEngine *gEng;

void handle_mouse_button(SDL_MouseButtonEvent *lEvent)
{
	int lMouseN = lEvent->button-1;
	switch(lMouseN)
	{
		case MOUSE_1: case MOUSE_2: case MOUSE_3:
			gMouseButton[lEvent->button-1] = (lEvent->state == 0 ? true : false);
			break;
		default:
			break;
	}
}

void handle_mouse_motion(SDL_MouseMotionEvent *lEvent)
{
	gMousePos[MOUSE_X] = lEvent->x;
	gMousePos[MOUSE_Y] = lEvent->y;

	gcheckMouseCollide_ret = gEng->checkMouseCollide(gMousePos[MOUSE_X], gMousePos[MOUSE_Y]);
}

void handle_key_up(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
		case SDLK_ESCAPE:
			gEsc = true;
			break;
		case SDLK_UP:
			gArrow[ARROW_UP] = false;
			break;
		case SDLK_DOWN:
			gArrow[ARROW_DOWN] = false;
			break;
		case SDLK_RETURN:
			gEnter = true;
			break;
		default:
			break;
	}
}

void handle_key_down(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
		case SDLK_ESCAPE:
			gEsc = false;
			break;
		case SDLK_UP:
			gArrow[ARROW_UP] = true;
			break;
		case SDLK_DOWN:
			gArrow[ARROW_DOWN] = true;
			break;
		case SDLK_RETURN:
			gEnter = false;
			break;
		default:
			break;
	}
}

static void dBox(float l, float w, float h, GLubyte clr[])
{
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cBox(l/2.0f, w/2.0f, h/2.0f));
	glColor4ubv(clr);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
}

static void dBoxW(float l, float w, float h, GLubyte clr[])
{
	float ld2 = l/2.0f;
	float wd2 = w/2.0f;
	float hd2 = h/2.0f;

	glBegin(GL_LINE_LOOP);
		glColor4ubv(clr); glVertex3f(-ld2,  hd2, -wd2);
		glColor4ubv(clr); glVertex3f(-ld2,  hd2,  wd2);
		glColor4ubv(clr); glVertex3f( ld2,  hd2,  wd2);
		glColor4ubv(clr); glVertex3f( ld2,  hd2, -wd2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glColor4ubv(clr); glVertex3f(-ld2, -hd2, -wd2);
		glColor4ubv(clr); glVertex3f(-ld2, -hd2,  wd2);
		glColor4ubv(clr); glVertex3f( ld2, -hd2,  wd2);
		glColor4ubv(clr); glVertex3f( ld2, -hd2, -wd2);
	glEnd();

	glBegin(GL_LINES);
		glColor4ubv(clr); glVertex3f(-ld2,  hd2,  wd2);
		glColor4ubv(clr); glVertex3f(-ld2, -hd2,  wd2);

		glColor4ubv(clr); glVertex3f(-ld2,  hd2, -wd2);
		glColor4ubv(clr); glVertex3f(-ld2, -hd2, -wd2);


		glColor4ubv(clr); glVertex3f( ld2,  hd2,  wd2);
		glColor4ubv(clr); glVertex3f( ld2, -hd2,  wd2);

		glColor4ubv(clr); glVertex3f( ld2,  hd2, -wd2);
		glColor4ubv(clr); glVertex3f( ld2, -hd2, -wd2);
	glEnd();
}

static void dWBox(float l, float w, float h, GLubyte clr[], GLubyte clr_w[], float s)
{
	float lw = 0;
	glGetFloatv(GL_LINE_WIDTH, &lw);

	dBox(l, w, h, clr);

	glLineWidth(s);
	dBoxW(l, w, h, clr_w);
	glLineWidth(lw);
}

static void dButton(const wchar_t *text, GLubyte clr_b[], GLubyte clr_w[], GLubyte clr_t[])
{
	glPushMatrix();
		dWBox(3.0f, 0.2f, 1.0f, clr_b, clr_w, 2.0f);
		glTranslatef(0.0f, 0.0f, 0.1f);
		glScalef(0.6f, 0.6f, 0.6f);
		glColor3ubv(clr_t);
		gEng->Font->Print(text);
	glPopMatrix();
}

static int gId = -1;

static void drawMenu()
{
	gEng->set3D();

	const int lNum = 4;
	const float lAng = 10.0f;
	const wchar_t *lMenus[lNum] = {L"New game", L"Options", L"Credits", L"Quit"};

	static float lAngles[lNum];
	static bool  lDir[lNum];
	static bool lInited = false;
	static int lId = 0;
	static int lTmr = 0;

	if (!lInited)
	{
		for (int i = 0; i < lNum; i++)
		{
			lAngles[i] = -(rand() % 11) + 5;
			lDir[i] = (int)(rand()*11) % 2 == 0 ? true : false;
		}
		lInited = true;
	}

	if (gArrow[ARROW_DOWN] & lTmr == 0)
		lId = (lId+1) % 4;
	if (gArrow[ARROW_UP] & lTmr == 0)
		lId = (lId == 0 ? 3 : lId-1);

	if (gArrow[ARROW_DOWN] | gArrow[ARROW_UP])
		lTmr = (lTmr+1) % 500;
	else
		lTmr = 0;

	if (gcheckMouseCollide_ret > 0)
		lId = gcheckMouseCollide_ret-1;
	
	for (int i = 0; i < lNum; i++)
	{
		gEng->mBegin(i+1);
			glTranslatef(0.0f, 2.0f-(1.2f*i), -5.0f);
			glRotatef(lAngles[i], 0.0f, 1.0f, 0.0f);
			dButton(lMenus[i], i == lId ? red : black, white, white);
		gEng->mEnd();

		if (lAngles[i] >=  lAng)
			lDir[i] = true;
		if (lAngles[i] <= -lAng)
			lDir[i] = false;
		lAngles[i] += lDir[i] ? -0.001f : 0.001f;
	}
	if (gEnter | ((gMouseButton[MOUSE_1]) & (gcheckMouseCollide_ret > 0)))
	{
		gId = lId;
		gEnter = false;
		gMouseButton[MOUSE_1] = false;
	}

	if (gEsc)
		gEng->quit_prog(0);
}

static void drawCredits()
{
	static float lAngle = -(rand() % 11) + 5;
	static float lAng = 10.0f;
	static bool lDir = (int)(rand()*11) % 2 == 0 ? true : false;

	gEng->set2D();
	gEng->mBegin();
		glDisable(GL_DEPTH_TEST);
			gEng->Font->Print(L"Developer: B00sT\nGraphic libraries used: SDL, OpenGL, GLU\nOther libraries used: FTGL\nTools used: vim", true, 10, 10);
		glEnable(GL_DEPTH_TEST);
	gEng->mEnd();
	gEng->set3D();

	gEng->mBegin(1);
		glTranslatef(0.0f, -2.0f, -5.0f);
		glRotatef(lAngle, 0.0f, 1.0f, 0.0f);
		dButton(L"Back", gcheckMouseCollide_ret == 1 ? red : black, white, white);
	gEng->mEnd();

	if (lAngle >=  lAng)
		lDir = true;
	if (lAngle <= -lAng)
		lDir = false;
	lAngle += lDir ? -0.001f : 0.001f;

	if ((gEsc) | ((gMouseButton[MOUSE_1]) & (gcheckMouseCollide_ret == 1)))
	{
		gId = -1;
		gEsc = false;
		gMouseButton[MOUSE_1] = false;
	}
}

static void drawOptions()
{
	gEng->set3D();

	static float lAngle = -(rand() % 11) + 5;
	static float lAng = 10.0f;
	static bool lDir = (int)(rand()*11) % 2 == 0 ? true : false;

	gEng->set2D();
	gEng->mBegin();
		glDisable(GL_DEPTH);
			gEng->Font->Print(L"Not realized yet.", true, 10, 10);
		glEnable(GL_DEPTH);
	gEng->mEnd();
	gEng->set3D();

	gEng->mBegin(1);
		glTranslatef(0.0f, -2.0f, -5.0f);
		glRotatef(lAngle, 0.0f, 1.0f, 0.0f);
		dButton(L"Back", gcheckMouseCollide_ret == 1 ? red : black, white, white);
	gEng->mEnd();

	if (lAngle >=  lAng)
		lDir = true;
	if (lAngle <= -lAng)
		lDir = false;
	lAngle += lDir ? -0.001f : 0.001f;

	if ((gEsc) | ((gMouseButton[MOUSE_1]) & (gcheckMouseCollide_ret == 1)))
	{
		gId = -1;
		gEsc = false;
		gMouseButton[MOUSE_1] = false;
	}
}

static void draw_screen(void)
{
	gEng->clearScreen();

	switch(gId)
	{
		case -1:
			drawMenu();
			break;
		case 1:
			drawOptions();
			break;
		case 2:
			drawCredits();
			break;
		case 3:
			gEng->quit_prog(0);
			break;			
	}

	SDL_GL_SwapBuffers();
}

#ifdef _WIN32
#include <windows.h>
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
#else
int main()
#endif
{
	const SDL_VideoInfo* info = NULL;
	int bpp = 0;
	int flags = 0;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		gEng->quit_prog(1);
	}

	info = SDL_GetVideoInfo();

	if(!info) {
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
	        gEng->quit_prog(1);
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

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		gEng->quit_prog(1);
	}

	gEng = new mEngine();
	gEng->mInit(width, height, "arial.ttf", true, draw_screen);
	gEng->setHandlers(	handle_key_down,
				handle_key_up,
				handle_mouse_motion,
				handle_mouse_button);

	while(1)
	{
		gEng->process_events();
		draw_screen();
	}
}
