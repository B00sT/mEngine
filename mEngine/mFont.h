#ifndef __FONT_H
#define __FONT_H

#include <cmath>
#include <string>
#include <wchar.h>
#include <GL/gl.h>
#include <FTGL/ftgl.h>

class MFont
{
	public:
		MFont(const char *ttf, float F3DSize, float F3DDepth, int F2DSize, int F2DDepth);
		~MFont();
		FTFont *Font3D, *Font2D;
		void Print(const wchar_t *text, bool f2D = false, int x = 0, int y = 0, int hIndent = 10);
};

#endif
