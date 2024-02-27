// Grass7 Version

#include "stdafx.h"
#include "Resource.h"
#include "gr7version.h"

ChangelogClass ChangelogClassObjects;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

	if(Grass7API::Check::isGrass7() == 0) {
			INITCOMMONCONTROLSEX iccx;

			BOOL certerr = Grass7API::Check::VerifyEmbeddedSignature(L"C:\\Windows\\System32\\gr7api.dll");

			// Small easter-egg
			OutputDebugStringW(L"why are you debugging this application ya old chum?\n");
			HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
			// We load the branding string using the Grass7 API
			Grass7API::String::LoadOSBrandingString(ChangelogClassObjects.szBranding);

			// We launch the function for Shell About if "/changelog" parameter is not specified.
			if (wcsstr(lpCmdLine, L"/changelog") != 0) {
				ChangelogClass::Changelog(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
			}
			else {
				iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
				iccx.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
				InitCommonControlsEx(&iccx);
		
				return ShellAboutW(NULL, ChangelogClassObjects.szBranding, NULL, hIcon);
			}
	}
	std::vector< TCHAR* > abc;
	free(ChangelogClassObjects.ws);
	return 0;
}

// Changelog entry function
int ChangelogClass::Changelog(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangelogClassObjects.wSizeX = 900;
	ChangelogClassObjects.wSizeY = 600;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChangelogClass::WndProc;
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
	
	std::wstring szTitle = ChangelogClassObjects.szBranding;
	szTitle.append(L" Changelog");

	ChangelogClassObjects.hInst = hInstance;
	HWND hWnd = CreateWindowW(
		L"gr7versionCL",
		szTitle.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		ChangelogClassObjects.wSizeX, ChangelogClassObjects.wSizeY,
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

	WCHAR windirW[MAX_PATH];
	UINT errWinDir = GetWindowsDirectoryW(windirW, sizeof(windirW));
	if (errWinDir == 0) {
		MessageBoxW(NULL, L"GetWindowsDirectoryW returned 0", L"Error", MB_OK | MB_ICONQUESTION);
	}
	std::wstring windir = windirW;
	windir.append(L"\\changelog.txt");

	size_t numCharstr;

	std::ifstream file(windir);
	if (file.is_open()) {
		while (getline(file, ChangelogClassObjects.line)) {
			ChangelogClassObjects.ws = Grass7API::Convert::convertchar(ChangelogClassObjects.line.c_str());

			ChangelogClassObjects.abc.push_back(ChangelogClassObjects.ws);

			size_t strcharsize = ChangelogClassObjects.line.length();
			if (strcharsize < ChangelogClassObjects.numstrcharsize) {
				numCharstr = strcharsize;
			}
			else {
				ChangelogClassObjects.numstrcharsize = strcharsize;
			}
			ChangelogClassObjects.iLineCount++;
		}
	}
	file.close();
	memset(windirW, 0, sizeof(windirW));

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

LRESULT CALLBACK ChangelogClass::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		ChangelogClassObjects.yChar = tm.tmHeight + tm.tmExternalLeading;

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
		ChangelogClassObjects.si.cbSize = sizeof(ChangelogClassObjects.si);
		ChangelogClassObjects.si.fMask = SIF_RANGE | SIF_PAGE;
		ChangelogClassObjects.si.nMin = 0;
		ChangelogClassObjects.si.nMax = ChangelogClassObjects.iLineCount;
		ChangelogClassObjects.si.nPage = yClient / ChangelogClassObjects.yChar;
		SetScrollInfo(hwnd, SB_VERT, &ChangelogClassObjects.si, TRUE);

		// Set the horizontal scrolling range and page size. 
		ChangelogClassObjects.si.cbSize = sizeof(ChangelogClassObjects.si);
		ChangelogClassObjects.si.fMask = SIF_RANGE | SIF_PAGE;
		ChangelogClassObjects.si.nMin = 0;
		ChangelogClassObjects.si.nMax = 2 + xClientMax / xChar;
		ChangelogClassObjects.si.nPage = xClient / xChar;
		SetScrollInfo(hwnd, SB_HORZ, &ChangelogClassObjects.si, TRUE);

		return 0;
	case WM_HSCROLL:
		// Get all the vertial scroll bar information.
		ChangelogClassObjects.si.cbSize = sizeof(ChangelogClassObjects.si);
		ChangelogClassObjects.si.fMask = SIF_ALL;

		// Save the position for comparison later on.
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogClassObjects.si);
		xPos = ChangelogClassObjects.si.nPos;
		switch (LOWORD(wParam))
		{
			// User clicked the left arrow.
		case SB_LINELEFT:
			ChangelogClassObjects.si.nPos -= 1;
			break;

			// User clicked the right arrow.
		case SB_LINERIGHT:
			ChangelogClassObjects.si.nPos += 1;
			break;

			// User clicked the scroll bar shaft left of the scroll box.
		case SB_PAGELEFT:
			ChangelogClassObjects.si.nPos -= ChangelogClassObjects.si.nPage;
			break;

			// User clicked the scroll bar shaft right of the scroll box.
		case SB_PAGERIGHT:
			ChangelogClassObjects.si.nPos += ChangelogClassObjects.si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			ChangelogClassObjects.si.nPos = ChangelogClassObjects.si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		ChangelogClassObjects.si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &ChangelogClassObjects.si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogClassObjects.si);

		// If the position has changed, scroll the window.
		if (ChangelogClassObjects.si.nPos != xPos)
		{
			ScrollWindow(hwnd, xChar * (xPos - ChangelogClassObjects.si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_VSCROLL:
		// Get all the vertial scroll bar information.
		ChangelogClassObjects.si.cbSize = sizeof(si);
		ChangelogClassObjects.si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &ChangelogClassObjects.si);

		// Save the position for comparison later on.
		ChangelogClassObjects.yPos = ChangelogClassObjects.si.nPos;
		switch (LOWORD(wParam))
		{

			// User clicked the HOME keyboard key.
		case SB_TOP:
			ChangelogClassObjects.si.nPos = ChangelogClassObjects.si.nMin;
			break;

			// User clicked the END keyboard key.
		case SB_BOTTOM:
			ChangelogClassObjects.si.nPos = ChangelogClassObjects.si.nMax;
			break;

			// User clicked the top arrow.
		case SB_LINEUP:
			ChangelogClassObjects.si.nPos -= 1;
			break;

			// User clicked the bottom arrow.
		case SB_LINEDOWN:
			ChangelogClassObjects.si.nPos += 1;
			break;

			// User clicked the scroll bar shaft above the scroll box.
		case SB_PAGEUP:
			ChangelogClassObjects.si.nPos -= ChangelogClassObjects.si.nPage;
			break;

			// User clicked the scroll bar shaft below the scroll box.
		case SB_PAGEDOWN:
			ChangelogClassObjects.si.nPos += ChangelogClassObjects.si.nPage;
			break;

			// User dragged the scroll box.
		case SB_THUMBTRACK:
			ChangelogClassObjects.si.nPos = ChangelogClassObjects.si.nTrackPos;
			break;

		default:
			break;
		}

		// Set the position and then retrieve it.  Due to adjustments
		// by Windows it may not be the same as the value set.
		ChangelogClassObjects.si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &ChangelogClassObjects.si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &ChangelogClassObjects.si);

		// If the position has changed, scroll window and update it.
		if (ChangelogClassObjects.si.nPos != ChangelogClassObjects.yPos)
		{
			ScrollWindow(hwnd, 0, ChangelogClassObjects.yChar * (ChangelogClassObjects.yPos - ChangelogClassObjects.si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_MOUSEWHEEL:
		ChangelogClassObjects.hwnd1 = hwnd;
		return ChangelogClass::MouseScroll((short)HIWORD(wParam));

	case WM_PAINT:
		// Prepare the window for painting.
		hdc = BeginPaint(hwnd, &ps);

		hFont = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
		hTmp = (HFONT)SelectObject(hdc, hFont);

		// Get vertical scroll bar position.
		ChangelogClassObjects.si.cbSize = sizeof(si);
		ChangelogClassObjects.si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &ChangelogClassObjects.si);
		ChangelogClassObjects.yPos = ChangelogClassObjects.si.nPos;

		// Get horizontal scroll bar position.
		GetScrollInfo(hwnd, SB_HORZ, &ChangelogClassObjects.si);
		xPos = ChangelogClassObjects.si.nPos;

		// Find painting limits.
		FirstLine = max(0, ChangelogClassObjects.yPos + ps.rcPaint.top / ChangelogClassObjects.yChar);
		LastLine = ChangelogClassObjects.iLineCount - 1;

		for (i = FirstLine; i <= LastLine; i++)
		{
			x = xChar * (1 - xPos);
			y = ChangelogClassObjects.yChar * (i - ChangelogClassObjects.yPos);

			// Note that "55" in the following depends on the 
			// maximum size of an abc[] item. Also, you must include
			// strsafe.h to use the StringCchLength function.
			hr = StringCchLengthW(ChangelogClassObjects.abc[i], ChangelogClassObjects.numstrcharsize + 1, &abcLength);
			if ((FAILED(hr)) | (abcLength == NULL))
			{

			}


			// Write a line of text to the client area.
			TextOutW(hdc, x, y, ChangelogClassObjects.abc[i], (INT)abcLength);
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

LONG ChangelogClass::MouseScroll(int nDelta) {
	int nScrollLines;
	ChangelogClassObjects.si.cbSize = sizeof(si);
	ChangelogClassObjects.si.fMask = SIF_ALL;
	GetScrollInfo(ChangelogClassObjects.hwnd1, SB_VERT, &ChangelogClassObjects.si);

	ChangelogClassObjects.yPos = ChangelogClassObjects.si.nPos;

	SystemParametersInfoW(SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

	if(nDelta == 120) {
		ChangelogClassObjects.si.nPos -= nScrollLines;
	}

	if (nDelta == -120) {
		ChangelogClassObjects.si.nPos += nScrollLines;
	}

	ChangelogClassObjects.si.fMask = SIF_POS;

	SetScrollInfo(ChangelogClassObjects.hwnd1, SB_VERT, &ChangelogClassObjects.si, TRUE);
	GetScrollInfo(ChangelogClassObjects.hwnd1, SB_VERT, &ChangelogClassObjects.si);

	if (ChangelogClassObjects.si.nPos != ChangelogClassObjects.yPos)
	{
		ScrollWindow(ChangelogClassObjects.hwnd1, 0, ChangelogClassObjects.yChar * (ChangelogClassObjects.yPos - ChangelogClassObjects.si.nPos), NULL, NULL);
		UpdateWindow(ChangelogClassObjects.hwnd1);
	}
	return 0;
}