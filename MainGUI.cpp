#include "stdafx.h"
#include "MainGUI.h"
#include "ButtonGUI.h"
#include "KeyHandler.h"
#include "ChangelogGUI.h"

MainGUI MainGUIObjects;

BOOL MainGUI::Init()
{
	NONCLIENTMETRICSW ncm;
	ZeroMemory(&ncm, sizeof(NONCLIENTMETRICSW));
	ncm.cbSize = sizeof(NONCLIENTMETRICSW);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, FALSE);
	MainObjects.hfDefault = CreateFontIndirectW(&ncm.lfMessageFont);

	MainGUIObjects.szTitle.append(L"About " + BrandingStringsObjects.GenericBrandingText);

	MainGUI::PopulateVersionText();

	MainGUIObjects.FirstLine = 0;
	MainGUIObjects.LastLine = (int)MainGUIObjects.VersionText.size() - 1;

	MainGUIObjects.wSizeX = 474;
	MainGUIObjects.wSizeY = 412;

	MainObjects.hWndMainWindow = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"gr7About",
		MainGUIObjects.szTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		MainGUIObjects.wSizeX, MainGUIObjects.wSizeY,
		nullptr,
		nullptr,
		MainObjects.hInst,
		nullptr
	);

	if (!MainObjects.hWndMainWindow) {
		MessageBoxW(nullptr, L"Failed to create the Main Window", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}

	DWORD dwStyle = GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE);
	DWORD dwRemove = WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, dwNewStyle);

	MainGUIObjects.hWndStaticBar = CreateWindowW(L"STATIC", nullptr, SS_ETCHEDHORZ | WS_CHILD | WS_VISIBLE, 15, 83, 426, 2, MainObjects.hWndMainWindow, (HMENU)IDC_STATIC, MainObjects.hInst, nullptr);

	if (!MainGUIObjects.hWndStaticBar) {
		MessageBoxW(nullptr, L"The static bar failed to be created", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}

	if (ButtonGUI::InitOKBtn()) {
		return 1;
	}

	if (ButtonGUI::InitChangelogBtn()) {
		return 1;
	}

	ShowWindow(MainObjects.hWndMainWindow, SW_SHOW);
	UpdateWindow(MainObjects.hWndMainWindow);

	return 0;
}

LRESULT CALLBACK MainGUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		KeyHandler::InvokeKeyHandler(wParam);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		SetFocus(hWnd);
	}
	break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_OKBTN:
			DestroyWindow(hWnd);
			break;
		case ID_CHANGELOGBTN:
			{
			if (!IsWindow(MainObjects.hWndChangelogWindow))
				ChangelogGUI::Launch();
			}
			break;
		default:
			return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT     ps;
		HDC             hdc;
		BITMAP			Bitmap;
		COLORREF		color;
		HDC				hdcMem;
		HBITMAP			oldBitmap;
		RECT			rchWnd;
		int xPos, yPos, yPosSpacing, nSize, yPosOffset, i;
	{
		hdc = BeginPaint(hWnd, &ps);

		// paint bitmap
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = (HBITMAP)SelectObject(hdcMem, BitmapObjects.hBanner);
		GetObjectW(BitmapObjects.hBanner, sizeof(Bitmap), &Bitmap);
		BitBlt(hdc, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);

		DeleteDC(hdcMem);
		DeleteObject(oldBitmap);

		// Text painting options
		xPos = 53;
		yPos = 100;
		yPosSpacing = 16;
		color = RGB(0, 0, 0);
		nSize = 8;
		yPosOffset = 0;

		GetClientRect(hWnd, &rchWnd);
		for (i = MainGUIObjects.FirstLine; i <= MainGUIObjects.LastLine; i++)
		{
			Grass7API::Paint::PaintText(hdc, xPos, yPos + yPosOffset, L"Tahoma", color, MainGUIObjects.VersionText[i].c_str(), nSize, 1, TRANSPARENT, FW_LIGHT, &rchWnd);
			yPosOffset = yPosOffset + yPosSpacing;
		}

		Grass7API::Paint::PaintText(hdc, xPos, yPos + 159, L"Tahoma", color, L"Name and Organization:", nSize, 1, TRANSPARENT, FW_LIGHT, &rchWnd);
		Grass7API::Paint::PaintText(hdc, xPos + 15, yPos + 159 + 19, L"Tahoma", color, MainGUIObjects.RegisteredOwner.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT, &rchWnd);
		Grass7API::Paint::PaintText(hdc, xPos + 15, yPos + 159 + 19 + 17, L"Tahoma", color, MainGUIObjects.RegisteredOrganization.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT, &rchWnd);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

BOOL MainGUI::Register()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"gr7About";
	wcex.hIconSm = nullptr;

	if (!RegisterClassExW(&wcex))
	{
		MessageBoxW(nullptr, L"Failed to register gr7About window class", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}
	return 0;
}

void MainGUI::PopulateVersionText()
{
	// Init variables
	std::wstring BuildString(MAX_PATH, 0);
	std::wstring CurrentBuild(MAX_PATH, 0);
	std::wstring RegisteredOwner(MAX_PATH, 0);
	std::wstring RegisteredOrganization(MAX_PATH, 0);
	
	// Load system information strings
	Grass7API::SystemInfo::GetBuildString(&BuildString[0], (int)BuildString.size());
	Grass7API::SystemInfo::GetCurrentBuild(&CurrentBuild[0], (int)CurrentBuild.size());
	Grass7API::SystemInfo::GetRegisteredOwner(&RegisteredOwner[0], (int)RegisteredOwner.size());
	Grass7API::SystemInfo::GetRegisteredOrganization(&RegisteredOrganization[0], (int)RegisteredOrganization.size());

	// Set registered system owner/organization strings
	MainGUIObjects.RegisteredOwner = RegisteredOwner;
	MainGUIObjects.RegisteredOrganization = RegisteredOrganization;

	std::wstring szBuildInfo = L"Build ";
	szBuildInfo.append(BuildString);

	// Put strings into VersionText array
	MainGUIObjects.VersionText.push_back(BrandingStringsObjects.ShortBrandingText);
	MainGUIObjects.VersionText.push_back(szBuildInfo);
	MainGUIObjects.VersionText.push_back(BrandingStringsObjects.CopyrightBrandingText);
}