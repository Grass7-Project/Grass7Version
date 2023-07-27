// Grass7 Version

#include "stdafx.h"
#include "Resource.h"
#include "gr7version.h"

static TCHAR szWindowClass[] = L"gr7versionCL";
int wSizeX = 900;
int wSizeY = 600;
size_t numstrcharsize;
int iLineCount = 0;

std::string line;
std::vector< TCHAR* > abc;
wchar_t *ws;

HINSTANCE hInst;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

	if(gr7::isGrass7() == 0) {
			INITCOMMONCONTROLSEX iccx;
			char buffer[50];

			// Small easter-egg
			OutputDebugStringW(L"why are you debugging this application ya old chum?\n");
			HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
			wcstombs_s(NULL, buffer, lpCmdLine, 50);
			char* buffer2 = buffer;
			std::string bf2 = buffer2;
			// We launch the function for Shell About if "/changelog" parameter is not specified.
			if((bf2.compare("/changelog")) == 0) {
				Changelog(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
			}
			else {
				iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
				iccx.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
				InitCommonControlsEx(&iccx);
		
				return ShellAboutW(NULL, gr7::LoadOSBrandingString(), NULL, hIcon);
			}
	}
	std::vector< TCHAR* > abc;
	line.clear();
	free(ws);
	return 0;
}

// Changelog entry function
int Changelog(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));

	if (!RegisterClassExW(&wcex))
	{
		MessageBoxW(NULL,
			L"Call to RegisterClassEx failed!",
			L"Error",
			NULL);

		return 1;
	}
	TCHAR szTitle[256] = { 0 };
	// We load the branding string using the Grass7 API
	wchar_t *szBranding = gr7::LoadOSBrandingString();
	wchar_t *szText = L" Changelog";
	size_t size1 = sizeof(szText);
	size_t size2 = sizeof(szBranding);
	size_t totalsize = size1 + size2;
	wcscpy_s(szTitle, szBranding);
	wcsncat_s(szTitle, szText, totalsize);

	hInst = hInstance;
	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wSizeX, wSizeY,
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

	wchar_t windir[256];
	char windirB[256] = { 0 };
	UINT errWinDir = GetWindowsDirectoryW(windir, sizeof(windir));
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
	}
	std::string windirS = gr7::WStringToString(windir);
	const char *windirCH(windirS.c_str());
	strncpy_s(windirB, windirCH, sizeof(windirB));
	strncat_s(windirB, "\\changelog.txt", sizeof(windirB));

	size_t numCharstr;

	std::ifstream file(windirB);
	if (file.is_open()) {
		while (getline(file, line)) {
			ws = gr7::convertchar(line.c_str());

			abc.push_back(ws);

			size_t strcharsize = line.length();
			if (strcharsize < numstrcharsize) {
				numCharstr = strcharsize;
			}
			else {
				numstrcharsize = strcharsize;
			}
			iLineCount++;
		}
	}
	file.close();
	memset(windir, 0, sizeof(windir));
	memset(windirB, 0, sizeof(windirB));
	windirS.clear();

	ShowWindow(hWnd,
		nCmdShow);
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
SCROLLINFO si;
static int yPos;
static int yChar;
HWND hwnd1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		yChar = tm.tmHeight + tm.tmExternalLeading;

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
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = iLineCount;
		si.nPage = yClient / yChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// Set the horizontal scrolling range and page size. 
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + xClientMax / xChar;
		si.nPage = xClient / xChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;
	case WM_HSCROLL:
		// Get all the vertial scroll bar information.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		// Save the position for comparison later on.
		GetScrollInfo(hwnd, SB_HORZ, &si);
		xPos = si.nPos;
		switch (LOWORD(wParam))
		{
			// User clicked the left arrow.
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

			// User clicked the right arrow.
		case SB_LINERIGHT:
			si.nPos += 1;
			break;

			// User clicked the scroll bar shaft left of the scroll box.
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

			// User clicked the scroll bar shaft right of the scroll box.
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// If the position has changed, scroll the window.
		if (si.nPos != xPos)
		{
			ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_VSCROLL:
		// Get all the vertial scroll bar information.
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		// Save the position for comparison later on.
		yPos = si.nPos;
		switch (LOWORD(wParam))
		{

			// User clicked the HOME keyboard key.
		case SB_TOP:
			si.nPos = si.nMin;
			break;

			// User clicked the END keyboard key.
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

			// User clicked the top arrow.
		case SB_LINEUP:
			si.nPos -= 1;
			break;

			// User clicked the bottom arrow.
		case SB_LINEDOWN:
			si.nPos += 1;
			break;

			// User clicked the scroll bar shaft above the scroll box.
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

			// User clicked the scroll bar shaft below the scroll box.
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);

		// If the position has changed, scroll window and update it.
		if (si.nPos != yPos)
		{
			ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_MOUSEWHEEL:
		hwnd1 = hwnd;
		return MouseScroll((short)HIWORD(wParam));

	case WM_PAINT:
		// Prepare the window for painting.
		hdc = BeginPaint(hwnd, &ps);

		hFont = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
		hTmp = (HFONT)SelectObject(hdc, hFont);

		// Get vertical scroll bar position.
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		yPos = si.nPos;

		// Get horizontal scroll bar position.
		GetScrollInfo(hwnd, SB_HORZ, &si);
		xPos = si.nPos;

		// Find painting limits.
		FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
		LastLine = iLineCount - 1;

		for (i = FirstLine; i <= LastLine; i++)
		{
			x = xChar * (1 - xPos);
			y = yChar * (i - yPos);

			// Note that "55" in the following depends on the 
			// maximum size of an abc[] item. Also, you must include
			// strsafe.h to use the StringCchLength function.
			hr = StringCchLengthW(abc[i], numstrcharsize + 1, &abcLength);
			if ((FAILED(hr)) | (abcLength == NULL))
			{

			}


			// Write a line of text to the client area.
			TextOutW(hdc, x, y, abc[i], (INT)abcLength);
		}

		// Indicate that painting is finished.
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

LONG MouseScroll(int nDelta) {
	int nScrollLines;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(hwnd1, SB_VERT, &si);

	yPos = si.nPos;

	SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

	if(nDelta == 120) {
		si.nPos -= nScrollLines;
	}

	if (nDelta == -120) {
		si.nPos += nScrollLines;
	}

	si.fMask = SIF_POS;

	SetScrollInfo(hwnd1, SB_VERT, &si, TRUE);
	GetScrollInfo(hwnd1, SB_VERT, &si);

	if (si.nPos != yPos)
	{
		ScrollWindow(hwnd1, 0, yChar * (yPos - si.nPos), NULL, NULL);
		UpdateWindow(hwnd1);
	}
	return 0;
}