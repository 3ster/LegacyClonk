/* by Sven2, 2003 */
// startup screen

#pragma once

#include <C4FacetEx.h>

class C4LoaderScreen
{
public:
	CStdFont &TitleFont; // font used for title output
	CStdFont &LogFont;   // font used for logging
	C4FacetExSurface fctBackground; // background image
	char *szInfo;      // info text to be drawn on loader screen

public:
	C4LoaderScreen();
	~C4LoaderScreen();

	bool Init(const char *szLoaderSpec); // inits and loads from global C4Game-class
	int SeekLoaderScreens(C4Group &rFromGrp, const char *szWildcard, int iLoaderCount, char *szDstName, C4Group **ppDestGrp);

	void Draw(C4Facet &cgo, int iProgress = 0, class C4LogBuffer *pLog = nullptr, int Process = 0); // draw loader screen (does not page flip!)
};