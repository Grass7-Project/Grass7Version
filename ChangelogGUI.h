#pragma once

class ChangelogGUI {
public:
	static int Init();
	static BOOL Register();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int wSizeX,
		wSizeY;

	std::wstring szTitle;
};