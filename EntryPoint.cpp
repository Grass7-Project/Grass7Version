#include "stdafx.h"
#include "MainInit.h"

GlobalMain MainObjects;
GlobalButtons ButtonObjects;
GlobalAppResStrings AppResStringsObjects;
GlobalBrandingStrings BrandingStringsObjects;
GlobalBitmaps BitmapObjects;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	int ret = MainInit::Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return ret;
}