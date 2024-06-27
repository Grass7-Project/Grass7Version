#include "stdafx.h"
#include "ResourceLoader.h"

void ResourceLoader::LoadBitmaps()
{
	// Normal Button States
	BitmapObjects.hNormalBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Pressed Button States
	BitmapObjects.hNormalBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Disabled Button States
	BitmapObjects.hNormalBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Hover Button States
	BitmapObjects.hNormalBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
}

// Load strings
void ResourceLoader::LoadStrings()
{
	// Init variables
	std::wstring NextButtonText(MAX_PATH, 0);

	// Button strings
	NextButtonText.resize(LoadStringW(MainObjects.hInst, IDS_NEXTBTN, &NextButtonText[0], (int)NextButtonText.size()));

	// Set loaded wstrings
	AppResStringsObjects.NextButtonText = NextButtonText;
}