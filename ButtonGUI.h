#pragma once

class ButtonGUI {
public:
	static void				InitMainWndScreenshot();
	static void				InitButtonBitmaps();
	static void				InitNormalBtn();

private:
	static void				Paint(HWND &hWnd, HBITMAP &hButtonImg, int xBmpPos, int yBmpPos, int drawText, LPCWSTR text, int textX, int textY);
	static void				ChangeBitmapState(HWND &hWnd, BOOL& ButtonDisabled, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, BOOL setState);
	static LRESULT CALLBACK	NormalButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

	// Temporary bitmaps, used for making the buttons look transparent
	HBITMAP hNormalBtnTmpImg;

	HBITMAP hbmpWndScreenshot;
	HDC hdcWndScreenshot,
		hdcMainWnd;

	BOOLEAN NormalButtonTracking,
		NormalButtonHover;
};