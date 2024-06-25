#include "stdafx.h"
#include "ChangelogGUI.h"

ChangelogGUI ChangelogGUIObjects;

// Changelog entry function
int ChangelogGUI::Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangelogGUIObjects.wSizeX = 900;
	ChangelogGUIObjects.wSizeY = 600;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChangelogGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
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

	ChangelogGUIObjects.hInst = hInstance;
	HWND hWnd = CreateWindowW(
		L"gr7versionCL",
		szTitle.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		ChangelogGUIObjects.wSizeX, ChangelogGUIObjects.wSizeY,
		NULL,
		NULL,
		hInstance,
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

	std::wstring windirW(MAX_PATH, 0);
	UINT errWinDir = GetWindowsDirectoryW(&windirW[0], MAX_PATH);
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
		return 1;
	}
	std::wstring windir = windirW.c_str();
	windir.append(L"\\changelog.txt");

	size_t numCharstr;

	std::wifstream file(windir);
	if (file.is_open()) {
		while (getline(file, ChangelogGUIObjects.line)) {

			ChangelogGUIObjects.abc.push_back(ChangelogGUIObjects.line.c_str());

			size_t strcharsize = ChangelogGUIObjects.line.length();
			if (strcharsize < ChangelogGUIObjects.numstrcharsize) {
				numCharstr = strcharsize;
			}
			else {
				ChangelogGUIObjects.numstrcharsize = strcharsize;
			}
			ChangelogGUIObjects.iLineCount++;
		}
	}
	file.close();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	RECT rc = { 0 };
	GetClientRect(hWnd, &rc);
	SCROLLINFO si = { 0 };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = 45;
	si.nPage = (rc.bottom - rc.top);
	si.nPos = 0;
	si.nTrackPos = 0;
	SetScrollInfo(hWnd, SB_VERT, &si, true);

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
	TEXTMETRIC tm;
	HFONT hFont, hTmp;

	// These variables are required to display text. 
	static int xClient;     // width of client area 
	static int yClient;     // height of client area 
	static int xClientMax;  // maximum width of client area 

	static int xChar;       // horizontal scrolling unit 
	static int xUpper;      // average width of uppercase letters 

	static int xPos;        // current horizontal scrolling position 

	int i;                  // loop counter 
	int x, y;               // horizontal and vertical coordinates

	int FirstLine;          // first line in the invalidated area 
	int LastLine;           // last line in the invalidated area 
	HRESULT hr;
	size_t abcLength;        // length of an abc[] item 

	switch (uMsg)
	{
	case WM_CREATE:
		// Get the handle to the client area's device context. 
		hdc = GetDC(hwnd);
		// Extract font dimensions from the text metrics. 
		GetTextMetricsW(hdc, &tm);
		xChar = tm.tmAveCharWidth;
		xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar / 2;
		ChangelogGUIObjects.yChar = tm.tmHeight + tm.tmExternalLeading;

		// Free the device context. 
		ReleaseDC(hwnd, hdc);

		// Set an arbitrary maximum width for client area. 
		// (xClientMax is the sum of the widths of 48 average 
		// lowercase letters and 12 uppercase letters.) 
		xClientMax = 103 * xChar + 12 * xUpper;

		return 0;

	case WM_SIZE:

		// Retrieve the dimensions of the client area. 
		yClient = HIWORD(lParam);
		xClient = LOWORD(lParam);

		// Set the vertical scrolling range and page size
		ChangelogGUIObjects.si.cbSize = sizeof(ChangelogGUIObjects.si);
		ChangelogGUIObjects.si.fMask = SIF_RANGE | SIF_PAGE;
		ChangelogGUIObjects.si.nMin = 0;
		ChangelogGUIObjects.si.nMax = ChangelogGUIObjects.iLineCount;
		ChangelogGUIObjects.si.nPage = yClient / ChangelogGUIObjects.yChar;
		SetScrollInfo(hwnd, SB_VERT, &ChangelogGUIObjects.si, TRUE);

		// Set the horizontal scrolling range and page size. 
		ChangelogGUIObjects.si.cbSize = sizeof(ChangelogGUIObjects.si);
		ChangelogGUIObjects.si.fMask = SIF_RANGE | SIF_PAGE;
		ChangelogGUIObjects.si.nMin = 0;
		ChangelogGUIObjects.si.nMax = 2 + xClientMax / xChar;
		ChangelogGUIObjects.si.nPage = xClient / xChar;
		SetScrollInfo(hwnd, SB_HORZ, &ChangelogGUIObjects.si, TRUE);

		return 0;
	case WM_HSCROLL:
		// Get all the vertial scroll bar information.
		ChangelogGUIObjects.si.cbSize = sizeof(ChangelogGUIObjects.si);
		ChangelogGUIObjects.si.fMask = SIF_ALL;

		// Save the position for comparison later on.
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogGUIObjects.si);
		xPos = ChangelogGUIObjects.si.nPos;
		switch (LOWORD(wParam))
		{
			// User clicked the left arrow.
		case SB_LINELEFT:
			ChangelogGUIObjects.si.nPos -= 1;
			break;

			// User clicked the right arrow.
		case SB_LINERIGHT:
			ChangelogGUIObjects.si.nPos += 1;
			break;

			// User clicked the scroll bar shaft left of the scroll box.
		case SB_PAGELEFT:
			ChangelogGUIObjects.si.nPos -= ChangelogGUIObjects.si.nPage;
			break;

			// User clicked the scroll bar shaft right of the scroll box.
		case SB_PAGERIGHT:
			ChangelogGUIObjects.si.nPos += ChangelogGUIObjects.si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			ChangelogGUIObjects.si.nPos = ChangelogGUIObjects.si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		ChangelogGUIObjects.si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &ChangelogGUIObjects.si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogGUIObjects.si);

		// If the position has changed, scroll the window.
		if (ChangelogGUIObjects.si.nPos != xPos)
		{
			ScrollWindow(hwnd, xChar * (xPos - ChangelogGUIObjects.si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_VSCROLL:
		// Get all the vertial scroll bar information.
		ChangelogGUIObjects.si.cbSize = sizeof(si);
		ChangelogGUIObjects.si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &ChangelogGUIObjects.si);

		// Save the position for comparison later on.
		ChangelogGUIObjects.yPos = ChangelogGUIObjects.si.nPos;
		switch (LOWORD(wParam))
		{

			// User clicked the HOME keyboard key.
		case SB_TOP:
			ChangelogGUIObjects.si.nPos = ChangelogGUIObjects.si.nMin;
			break;

			// User clicked the END keyboard key.
		case SB_BOTTOM:
			ChangelogGUIObjects.si.nPos = ChangelogGUIObjects.si.nMax;
			break;

			// User clicked the top arrow.
		case SB_LINEUP:
			ChangelogGUIObjects.si.nPos -= 1;
			break;

			// User clicked the bottom arrow.
		case SB_LINEDOWN:
			ChangelogGUIObjects.si.nPos += 1;
			break;

			// User clicked the scroll bar shaft above the scroll box.
		case SB_PAGEUP:
			ChangelogGUIObjects.si.nPos -= ChangelogGUIObjects.si.nPage;
			break;

			// User clicked the scroll bar shaft below the scroll box.
		case SB_PAGEDOWN:
			ChangelogGUIObjects.si.nPos += ChangelogGUIObjects.si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			ChangelogGUIObjects.si.nPos = ChangelogGUIObjects.si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		ChangelogGUIObjects.si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &ChangelogGUIObjects.si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &ChangelogGUIObjects.si);

		// If the position has changed, scroll window and update it.
		if (ChangelogGUIObjects.si.nPos != ChangelogGUIObjects.yPos)
		{
			ScrollWindow(hwnd, 0, ChangelogGUIObjects.yChar * (ChangelogGUIObjects.yPos - ChangelogGUIObjects.si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_MOUSEWHEEL:
		ChangelogGUIObjects.hwndMouse = hwnd;
		return ChangelogGUI::MouseScroll((short)HIWORD(wParam));

	case WM_PAINT:
		// Prepare the window for painting.
		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		ChangelogGUIObjects.nHeight = -MulDiv(18, GetDeviceCaps(hdc, LOGPIXELSY), 72);

		hFont = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
		hTmp = (HFONT)SelectObject(hdc, hFont);

		// Get vertical scroll bar position.
		ChangelogGUIObjects.si.cbSize = sizeof(si);
		ChangelogGUIObjects.si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &ChangelogGUIObjects.si);
		ChangelogGUIObjects.yPos = ChangelogGUIObjects.si.nPos;

		// Get horizontal scroll bar position.
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogGUIObjects.si);
		xPos = ChangelogGUIObjects.si.nPos;

		// Find painting limits.
		FirstLine = max(0, ChangelogGUIObjects.yPos + ps.rcPaint.top / ChangelogGUIObjects.yChar);
		LastLine = ChangelogGUIObjects.iLineCount - 1;

		for (i = FirstLine; i <= LastLine; i++)
		{
			x = xChar * (1 - xPos);
			y = ChangelogGUIObjects.yChar * (i - ChangelogGUIObjects.yPos);

			// Note that "55" in the following depends on the 
			// maximum size of an abc[] item. Also, you must include
			// strsafe.h to use the StringCchLength function.
			hr = StringCchLengthW(ChangelogGUIObjects.abc[i].c_str(), ChangelogGUIObjects.numstrcharsize + 1, &abcLength);
			if ((FAILED(hr)) | (abcLength == NULL))
			{
			}

			// Write a line of text to the client area.
			TextOutW(hdc, x, y, ChangelogGUIObjects.abc[i].c_str(), (INT)abcLength);
		}

		DeleteObject(hFont);
		DeleteObject(hTmp);

		// Indicate that painting is finished.
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LONG ChangelogGUI::MouseScroll(int nDelta) {
	int nScrollLines;
	ChangelogGUIObjects.si.cbSize = sizeof(si);
	ChangelogGUIObjects.si.fMask = SIF_ALL;
	GetScrollInfo(ChangelogGUIObjects.hwndMouse, SB_VERT, &ChangelogGUIObjects.si);

	ChangelogGUIObjects.yPos = ChangelogGUIObjects.si.nPos;

	SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

	if (nDelta == 120) {
		ChangelogGUIObjects.si.nPos -= nScrollLines;
	}

	if (nDelta == -120) {
		ChangelogGUIObjects.si.nPos += nScrollLines;
	}

	ChangelogGUIObjects.si.fMask = SIF_POS;

	SetScrollInfo(ChangelogGUIObjects.hwndMouse, SB_VERT, &ChangelogGUIObjects.si, TRUE);
	GetScrollInfo(ChangelogGUIObjects.hwndMouse, SB_VERT, &ChangelogGUIObjects.si);

	if (ChangelogGUIObjects.si.nPos != ChangelogGUIObjects.yPos)
	{
		ScrollWindow(ChangelogGUIObjects.hwndMouse, 0, ChangelogGUIObjects.yChar * (ChangelogGUIObjects.yPos - ChangelogGUIObjects.si.nPos), NULL, NULL);
		UpdateWindow(ChangelogGUIObjects.hwndMouse);
	}
	return 0;
}