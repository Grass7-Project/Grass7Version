#include "stdafx.h"
#include "ExitHandler.h"

void ExitHandler::Cleanup()
{
	DestroyWindow(MainObjects.hWndRichEditCtrl);
	FreeLibrary(MainObjects.hModRichEdit);
	DeleteObject(MainObjects.hfDefault);
	DeleteObject(BitmapObjects.hBanner);
}