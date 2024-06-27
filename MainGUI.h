#pragma once

class MainGUI {
public:
	static int Init(HWND &hWnd);

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int wSizeX,
		wSizeY;

	HBITMAP hBitmap;

	HWND hWndStaticBar;
};