#include "stdafx.h"
#include "MainGUI.h"
#include "ResourceLoader.h"
#include "ButtonGUI.h"

MainGUI MainGUIObjects;

// Changelog entry function
int MainGUI::Init(HWND &hWnd)
{
	Grass7API::Branding::LoadOSBrandingImage(MainGUIObjects.hBitmap);
	MainGUIObjects.hBitmap = Grass7API::Paint::ReplaceColor(MainGUIObjects.hBitmap, RGB(255, 255, 255), RGB(240, 240, 240), NULL);

	// Call the function to load the bitmaps
	ResourceLoader::LoadBitmaps();

	// Load strings
	ResourceLoader::LoadStrings();

	MainGUIObjects.wSizeX = 474;
	MainGUIObjects.wSizeY = 412;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"gr7versionCL";
	wcex.hIconSm = NULL;

	if (!RegisterClassExW(&wcex))
	{
		MessageBoxW(NULL,
			L"Call to RegisterClassEx failed!",
			L"Error",
			NULL);

		return 1;
	}

	std::wstring szTitle = L"About ";
	szTitle.append(MainObjects.szBranding);

	hWnd = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"gr7versionCL",
		szTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		MainGUIObjects.wSizeX, MainGUIObjects.wSizeY,
		NULL,
		NULL,
		MainObjects.hInst,
		NULL
	);

	if (!hWnd)
	{
		MessageBoxW(NULL,
			L"Call to CreateWindow failed!",
			L"Error",
			NULL);
		return 1;
	}

	DWORD dwStyle = GetWindowLongW(hWnd, GWL_STYLE);
	DWORD dwRemove = WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	SetWindowLongW(hWnd, GWL_STYLE, dwNewStyle);

	MainGUIObjects.hWndStaticBar = CreateWindowW(L"STATIC", NULL, SS_ETCHEDHORZ | WS_CHILD | WS_VISIBLE, 15, 83, 426, 2, hWnd, (HMENU)IDC_STATIC, MainObjects.hInst, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	ButtonGUI::InitMainWndScreenshot();
	ButtonGUI::InitNormalBtn();

	if (ButtonObjects.NormalButtonState == 3) {
		::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
	}
	::SendMessageW(ButtonObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK MainGUI::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	BITMAP Bitmap;

	switch (uMsg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, MainGUIObjects.hBitmap);
		GetObjectW(MainGUIObjects.hBitmap, sizeof(Bitmap), &Bitmap);
		BitBlt(hdc, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);

		DeleteDC(hdcMem);
		DeleteObject(oldBitmap);

		EndPaint(hwnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}