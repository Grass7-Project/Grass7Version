#include "stdafx.h"
#include "ButtonGUI.h"
#include "MainInit.h"
#include "MainGUI.h"

ButtonGUI BtnGUI;

// Initialize Normal Button
void ButtonGUI::InitNormalBtn()
{
	ButtonObjects.NormalButtonDisabled = FALSE;
	ButtonObjects.NormalButtonState = 1;

	ButtonObjects.hNormalBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 376, 343, 75, 23, MainObjects.hWndMainWindow, (HMENU)ID_NORMALBTN, MainObjects.hInst, 0);
	SetWindowSubclass(ButtonObjects.hNormalBtn, &ButtonGUI::NormalButtonProc, ID_NORMALBTN, 0);
}

void ButtonGUI::InitMainWndScreenshot()
{
	int height;
	int width;
	Grass7API::Monitor::GetDesktopResolution(width, height);

	BitBlt(BtnGUI.hdcWndScreenshot, 0, 0, width, height, BtnGUI.hdcMainWnd, 0, 0, SRCCOPY);
}

// Initialize Button Bitmaps for use
void ButtonGUI::InitButtonBitmaps()
{
	int height;
	int width;
	Grass7API::Monitor::GetDesktopResolution(width, height);
	BtnGUI.hdcMainWnd = ::GetDC(MainObjects.hWndMainWindow);
	BtnGUI.hdcWndScreenshot = ::CreateCompatibleDC(BtnGUI.hdcMainWnd);
	BtnGUI.hbmpWndScreenshot = ::CreateCompatibleBitmap(BtnGUI.hdcMainWnd, width, height);
	SelectObject(BtnGUI.hdcWndScreenshot, BtnGUI.hbmpWndScreenshot);

	BtnGUI.hNormalBtnTmpImg = BitmapObjects.hNormalBtnImg1;
}

void ButtonGUI::Paint(HWND &hWnd, HBITMAP &hButtonImg, int xBmpPos = 0, int yBmpPos = 0, int drawText = FALSE, LPCWSTR text = L"", int textX = 0, int textY = 0)
{
	InvalidateRect(hWnd, 0, TRUE);
	BITMAP          bitmap01;
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rc;
	GetWindowRect(hWnd, &rc);
	GetObjectW(hButtonImg, sizeof(bitmap01), &bitmap01);

	hdc = BeginPaint(hWnd, &ps);

	BitBlt(hdc, xBmpPos, yBmpPos, bitmap01.bmWidth, bitmap01.bmHeight, BtnGUI.hdcWndScreenshot, rc.left + xBmpPos, rc.top + yBmpPos, SRCCOPY);

	Grass7API::Paint::PaintTransparentBitmap(hdc, xBmpPos, yBmpPos, hButtonImg, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

	if (drawText == TRUE) {
		if (textX == 0 && textY == 0) {
			Grass7API::Paint::PaintText(hdc, (rc.right - rc.left) / 2 - 12, (rc.bottom - rc.top) / 2 - 7, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
		if (textX != 0 && textY == 0) {
			Grass7API::Paint::PaintText(hdc, textX, (rc.bottom - rc.top) / 2 - 7, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
		if (textX == 0 && textY != 0) {
			Grass7API::Paint::PaintText(hdc, (rc.right - rc.left) / 2 - 12, textY, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
		if (textX != 0 && textY != 0) {
			Grass7API::Paint::PaintText(hdc, textX, textY, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
	}

	EndPaint(hWnd, &ps);
}

void ButtonGUI::ChangeBitmapState(HWND &hWnd, BOOL &ButtonDisabled, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, BOOL setState = FALSE)
{
	ButtonDisabled = setState;
	hButtonTmpImg = hButtonImg;
	::RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT);
}

// Normal Button Procedure
LRESULT CALLBACK ButtonGUI::NormalButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		ButtonGUI::Paint(hWnd, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.NextButtonText.c_str());
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.NormalButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.NormalButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.NormalButtonHover = TRUE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.NormalButtonTracking = BtnGUI.NormalButtonHover = FALSE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		SetFocus(MainObjects.hWndMainWindow);
		return SendMessageW(MainObjects.hWndMainWindow, WM_KEYDOWN, wParam, lParam);
	}
	break;
	case BTN_DISABLE:
	{
		ButtonObjects.NormalButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonObjects.NormalButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1, FALSE);
	}
	break;
	case BTN_UPDATE:
	{
		if (ButtonObjects.NormalButtonState == 1) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg3, TRUE);
		}
		if (ButtonObjects.NormalButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hNormalBtn, &NormalButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}