#include "stdafx.h"
#include "MainInit.h"
#include "MainGUI.h"
#include "ChangelogGUI.h"

int MainInit::Init(MSG &msg, HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow)
{
	if (Grass7API::Check::isGrass7() == 0) {
		MainObjects.hInst = hInstance;

		OutputDebugStringW(L"why are you debugging this application ya old chum?\n");
		// Load branding string
		Grass7API::Branding::LoadOSBrandingString(MainObjects.szBranding, L"%WINDOWS_GENERIC%");

		// Launch the function for Shell About if "/changelog" parameter is not specified.
		if (wcsstr(lpCmdLine, L"/changelog") != 0) {
			ChangelogGUI::Init(MainObjects.hWndChangelogWindow);
		}
		else {
			MainGUI::Init(MainObjects.hWndMainWindow);

			/*INITCOMMONCONTROLSEX iccx;
			HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
			iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
			iccx.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
			InitCommonControlsEx(&iccx);

			return ShellAboutW(NULL, MainObjects.szBranding, NULL, hIcon);*/
		}
	}

	return 0;
}