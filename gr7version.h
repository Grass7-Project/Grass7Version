#pragma once

#include "resource.h"

class ChangelogClass{
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static int Changelog(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	static LONG MouseScroll(int nDelta);

	wchar_t *szBranding;
	wchar_t *ws;

private:
	int wSizeX,
		wSizeY,
		iLineCount;
	size_t numstrcharsize;

	std::string line;
	std::vector< TCHAR* > abc;

	HINSTANCE hInst;

	SCROLLINFO si;
	int yPos;
	int yChar;
	HWND hwnd1;
};