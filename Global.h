#pragma once

#include "resource.h"

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst;
	HWND hWndMainWindow;
	HWND hWndChangelogWindow;
	HWND hWndRichEditCtrl;
	HFONT hfDefault;
	wchar_t *szBranding;
};

// Buttons
class GlobalButtons {
public:
	HWND hOKBtn;
	HWND hChangelogBtn;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	std::wstring OKButtonText;
	std::wstring ChangelogButtonText;
	std::wstring ErrorTitleText;
};

// Bitmaps
class GlobalBitmaps {
public:
	HBITMAP hBanner;
};

extern GlobalMain MainObjects;
extern GlobalButtons ButtonObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalBitmaps BitmapObjects;