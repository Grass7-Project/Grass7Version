#pragma once

class MainGUI {
public:
	static int Init();
	static BOOL Register();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int wSizeX,
		wSizeY;

	HWND hWndStaticBar;
};