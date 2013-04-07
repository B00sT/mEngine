#include "mFont.h"

// -- Font constuctor
MFont::MFont(const char *ttf, float F3DSize, float F3DDepth, int F2DSize, int F2DDepth)
{
	// -- Init 3D font
	this->Font3D = new FTExtrudeFont(ttf);
	if(!Font3D->FaceSize(F3DSize))
	{
		printf("Error: Can't set font3d FaceSize");
		exit(1);
	}
	Font3D->Depth(F3DDepth);
	Font3D->CharMap(ft_encoding_unicode);

	// -- Init 2D font
	this->Font2D = new FTGLBitmapFont(ttf);
	if(!Font2D->FaceSize(F2DSize))
	{
		printf("Error: Can't set font2d FaceSize");
		exit(1);
	}
	Font2D->Depth(F2DDepth);
	Font2D->CharMap(ft_encoding_unicode);
}

void MFont::Print(const wchar_t *text, bool f2D, int x, int y, int hIndent)
{
	// -- Declare variables
	static FTFont *lFont;

	static std::wstring lStr;
	static int lStr_h;
	static bool fLastStr;

	// -- Init variables
	lStr = L"";
	lStr_h = 0;
	fLastStr = false;

	// -- If f2D is true, lFont will be set for render 2D text, else it will be
	// -- set for render 3D text
	if (f2D)
		lFont = Font2D;
	else
		lFont = Font3D;

	// -- Run cycle for string scan.
	int i = 0;
	do
	{
		// -- If text[i] is line break or null char(end of string)
		if ((text[i] == '\n') | (text[i] == '\0'))
		{
			glPushMatrix();
				// -- Width of string
				float lOffX = std::abs((lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Upper().X() - lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Lower().X()));
				// -- Height of string
				float lOffY = std::abs((lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Upper().Y() - lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Lower().Y()));
				// -- Length of string
				float lOffZ = std::abs((lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Upper().Z() - lFont->BBox(const_cast<wchar_t*>(lStr.c_str())).Lower().Z()));
				// -- If text render set for 2D rendering
				if (f2D)
				{
					// -- Move to (x, y)
					// -- x is x position of string
					// -- y is y position of string plus string
					// -- height plus lStr_h
					// -- lStr_h is a string height plus hIndent
					// -- hIndent is a indent between lines of
					// -- multiline string
					glRasterPos2f(x, y+lOffY+lStr_h);
					lStr_h += lOffY + hIndent;
				}
				// -- If text render set for 3D rendering, then render
				// -- it without multiline feature, because I don't
				// -- know how to convert raster indent to indent
				// -- for 3D field
				else
					glTranslatef(-lOffX/2.0f, -lOffY/2.0f, -lOffZ/2.0f);
				// -- Let's render text
				lFont->Render(const_cast<wchar_t*>(lStr.c_str()));
			glPopMatrix();
			// -- Let's empty the buffer for new scan
			lStr = L"";
		}
		else
			// -- If text[i] is not line break or null char, add text[i]
			// -- to our buffer(lStr)
			lStr += text[i];
		// -- Increase counter of cycle
		i++;
	} while (text[i-1] != '\0');
}
