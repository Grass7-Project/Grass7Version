#pragma once

#include "resource.h"

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst;
	HWND hWndMainWindow;
	HWND hWndChangelogWindow;
	HWND hWndRichEditCtrl;
	wchar_t *szBranding;
};

// Buttons
class GlobalButtons {
public:
	HWND hNormalBtn;

	int NormalButtonState;

	BOOL NormalButtonDisabled;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	std::wstring NextButtonText;
};

// Bitmaps
class GlobalBitmaps {
public:
	HBITMAP hNormalBtnImg1, // Normal Button Bitmaps
		hNormalBtnImg2,
		hNormalBtnImg3,
		hNormalBtnImg4;
};

extern GlobalMain MainObjects;
extern GlobalButtons ButtonObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalBitmaps BitmapObjects;