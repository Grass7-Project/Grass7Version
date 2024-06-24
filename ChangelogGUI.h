#pragma once

class ChangelogGUI {
public:
	static int Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static LONG MouseScroll(int nDelta);

	int wSizeX,
		wSizeY,
		iLineCount,
		nHeight,
		yPos,
		yChar;
	size_t numstrcharsize;

	std::wstring line;
	std::vector< std::wstring > abc;

	HINSTANCE hInst;

	SCROLLINFO si;
	HWND hwndMouse;
};