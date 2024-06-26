#pragma once

#include "resource.h"

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst;
	HWND hWndMainWindow;
	HWND hWndRichEditCtrl;
	wchar_t *szBranding;
};

extern GlobalMain MainObjects;