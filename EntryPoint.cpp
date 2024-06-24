#include "stdafx.h"
#include "Global.h"
#include "ChangelogGUI.h"

GlobalMain MainObjects;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

	if(Grass7API::Check::isGrass7() == 0) {
		INITCOMMONCONTROLSEX iccx;

		OutputDebugStringW(L"why are you debugging this application ya old chum?\n");
		// Load branding string
		Grass7API::String::LoadOSBrandingString(MainObjects.szBranding);

		// Launch the function for Shell About if "/changelog" parameter is not specified.
		if (wcsstr(lpCmdLine, L"/changelog") != 0) {
			ChangelogGUI::Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		}
		else {
			HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_GR7VERSION));
			iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
			iccx.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
			InitCommonControlsEx(&iccx);

			return ShellAboutW(NULL, MainObjects.szBranding, NULL, hIcon);
		}
	}
	return 0;
}