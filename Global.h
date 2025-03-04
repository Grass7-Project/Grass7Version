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
	HMODULE hModRichEdit;
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

// Branding Strings
class GlobalBrandingStrings {
public:
	std::wstring GenericBrandingText;
	std::wstring CopyrightBrandingText;
	std::wstring ShortBrandingText;
};

// Bitmaps
class GlobalBitmaps {
public:
	HBITMAP hBanner;
};

extern GlobalMain MainObjects;
extern GlobalButtons ButtonObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalBrandingStrings BrandingStringsObjects;
extern GlobalBitmaps BitmapObjects;