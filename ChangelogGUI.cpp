#include "stdafx.h"
#include "ChangelogGUI.h"

ChangelogGUI ChangelogGUIObjects;

// Changelog entry function
int ChangelogGUI::Init()
{
	ChangelogGUIObjects.wSizeX = 900;
	ChangelogGUIObjects.wSizeY = 600;

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
	wcex.lpszClassName = L"gr7versionCL";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));

	if (!RegisterClassExW(&wcex))
	{
		MessageBoxW(NULL,
			L"Call to RegisterClassEx failed!",
			L"Error",
			NULL);

		return 1;
	}

	std::wstring szTitle = MainObjects.szBranding;
	szTitle.append(L" Changelog");

	MainObjects.hWndMainWindow = CreateWindowW(
		L"gr7versionCL",
		szTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		ChangelogGUIObjects.wSizeX, ChangelogGUIObjects.wSizeY,
		NULL,
		NULL,
		MainObjects.hInst,
		NULL
	);

	if (!MainObjects.hWndMainWindow)
	{
		MessageBoxW(NULL,
			L"Call to CreateWindow failed!",
			L"Error",
			NULL);
		return 1;
	}

	std::wstring windirW(MAX_PATH, 0);
	UINT errWinDir = GetWindowsDirectoryW(&windirW[0], MAX_PATH);
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		return 1;
	}
	std::wstring file = windirW.c_str();
	file.append(L"\\Changelog.rtf");

	ShowWindow(MainObjects.hWndMainWindow, SW_SHOW);
	UpdateWindow(MainObjects.hWndMainWindow);

	MainObjects.hWndRichEditCtrl = Grass7API::RichEditControl::CreateRichEdit(MainObjects.hWndMainWindow, 0, 0, ChangelogGUIObjects.wSizeX, ChangelogGUIObjects.wSizeY, MainObjects.hInst);
	LONG lExStyle = GetWindowLong(MainObjects.hWndRichEditCtrl, GWL_EXSTYLE);
	lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLongW(MainObjects.hWndRichEditCtrl, GWL_EXSTYLE, lExStyle);
	SetWindowPos(MainObjects.hWndRichEditCtrl, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	Grass7API::RichEditControl::FillRichEditFromFile(MainObjects.hWndRichEditCtrl, file.c_str(), SF_RTF);
	::SendMessageW(MainObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK ChangelogGUI::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

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
		{
			hdc = BeginPaint(hwnd, &ps);

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