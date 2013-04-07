#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include <time.h>

#include "mFont.h"

class mEngine
{
public:
	struct vector3
	{
		GLdouble x,y,z;
		vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		vector3(GLdouble x_, GLdouble y_, GLdouble z_)
		{
			x = x_;
			y = y_;
			z = z_;
		}
	};
	mEngine();
	~mEngine();
	void set2D();
	void set3D();
	void quit_prog(int code);
	int checkMouseCollide(int mX, int mY);
	void process_events(void);
	void clearScreen();
	void mBegin(int name = 0);
	void mEnd();
	void setHandlers(
		void(*handle_key_down_func_)(SDL_keysym *keysym),
		void(*handle_key_up_func_)(SDL_keysym *keysym),
		void(*handle_mouse_motion_func_)(SDL_MouseMotionEvent *lEvent),
		void(*handle_mouse_button_func_)(SDL_MouseButtonEvent *lEvent));
	void mInit(int width, int height, const char *font_fname, bool fullScreen_, void(*draw_screen_func_)(void));

	MFont *Font;
	bool gfCheck;

private:
	int width;
	int height;

	void(*draw_screen_func)(void);
	void(*handle_key_down_func)(SDL_keysym *keysym);
	void(*handle_key_up_func)(SDL_keysym *keysym);
	void(*handle_mouse_motion_func)(SDL_MouseMotionEvent *lEvent);
	void(*handle_mouse_button_func)(SDL_MouseButtonEvent *lEvent);
};
