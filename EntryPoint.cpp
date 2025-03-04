#include "stdafx.h"
#include "MainInit.h"

GlobalMain MainObjects;
GlobalButtons ButtonObjects;
GlobalAppResStrings AppResStringsObjects;
GlobalBrandingStrings BrandingStringsObjects;
GlobalBitmaps BitmapObjects;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	return MainInit::Init(hInstance);
}