#include "stdafx.h"
#include "ChangelogGUI.h"

ChangelogGUI ChangelogGUIObjects;

// Changelog init function
int ChangelogGUI::Init()
{
	ChangelogGUIObjects.wSizeX = 900;
	ChangelogGUIObjects.wSizeY = 600;

	ChangelogGUIObjects.szTitle.append(BrandingStringsObjects.GenericBrandingText);
	ChangelogGUIObjects.szTitle.append(L" Changelog");

	std::wstring windirW(MAX_PATH, 0);
	UINT errWinDir = GetWindowsDirectoryW(&windirW[0], (int)windirW.size());
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"Failed to get path of the Windows directory", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}
	ChangelogGUIObjects.file = windirW.c_str();
	ChangelogGUIObjects.file.append(L"\\Changelog.rtf");

	return 0;
}

int ChangelogGUI::Launch()
{
	MainObjects.hWndChangelogWindow = NULL;
	MainObjects.hWndChangelogWindow = CreateWindowExW(
		NULL,
		L"gr7Changelog",
		ChangelogGUIObjects.szTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		ChangelogGUIObjects.wSizeX, ChangelogGUIObjects.wSizeY,
		MainObjects.hWndChangelogWindow,
		NULL,
		MainObjects.hInst,
		NULL
	);

	if (!MainObjects.hWndChangelogWindow)
	{
		MessageBoxW(NULL, L"Failed to create the Changelog Window", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}

	DWORD dwStyle = GetWindowLongW(MainObjects.hWndChangelogWindow, GWL_STYLE);
	DWORD dwRemove = WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	SetWindowLongW(MainObjects.hWndChangelogWindow, GWL_STYLE, dwNewStyle);

	ShowWindow(MainObjects.hWndChangelogWindow, SW_SHOW);
	UpdateWindow(MainObjects.hWndChangelogWindow);

	MainObjects.hWndRichEditCtrl = Grass7API::RichEditControl::CreateRichEdit(MainObjects.hWndChangelogWindow, 0, 0, ChangelogGUIObjects.wSizeX, ChangelogGUIObjects.wSizeY, MainObjects.hInst);
	LONG lExStyle = GetWindowLongW(MainObjects.hWndRichEditCtrl, GWL_EXSTYLE);
	lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLongW(MainObjects.hWndRichEditCtrl, GWL_EXSTYLE, lExStyle);
	SetWindowPos(MainObjects.hWndRichEditCtrl, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	Grass7API::RichEditControl::FillRichEditFromFile(MainObjects.hWndRichEditCtrl, ChangelogGUIObjects.file.c_str(), SF_RTF);
	::SendMessageW(MainObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);

	return 0;
}

LRESULT CALLBACK ChangelogGUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int xClient;     // width of client area 
	static int yClient;     // height of client area 

	switch (uMsg)
	{
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 662;
			lpMMI->ptMinTrackSize.y = 517;
		}
		break;
		case WM_SIZE:
		{
			// Retrieve the dimensions of the client area. 
			xClient = LOWORD(lParam);
			yClient = HIWORD(lParam);

			SetWindowPos(MainObjects.hWndRichEditCtrl, NULL, 0, 0, xClient, yClient, SWP_FRAMECHANGED);
		}
		break;
		case WM_PAINT:
			PAINTSTRUCT     ps;
			HDC             hdc;
			{
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

BOOL ChangelogGUI::Register()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChangelogGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_GR7VERSION));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"gr7Changelog";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));

	if (!RegisterClassExW(&wcex))
	{
		MessageBoxW(NULL, L"Failed to register gr7Changelog window class", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}
	return 0;
}