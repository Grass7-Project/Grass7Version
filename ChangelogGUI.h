#pragma once

class ChangelogGUI {
public:
	static BOOL Init();
	static BOOL Launch();
	static BOOL Register();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int wSizeX,
		wSizeY;

	std::wstring szTitle;
	std::wstring file;
};