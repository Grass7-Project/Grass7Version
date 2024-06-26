#pragma once

class ChangelogGUI {
public:
	static int Init();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int wSizeX,
		wSizeY;
};