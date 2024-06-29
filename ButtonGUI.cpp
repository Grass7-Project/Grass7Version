#include "stdafx.h"
#include "ButtonGUI.h"
#include "MainInit.h"
#include "MainGUI.h"

ButtonGUI BtnGUI;

// Initialize OK Button
BOOL ButtonGUI::InitOKBtn()
{
	ButtonObjects.hOKBtn = CreateWindowExW(NULL, L"BUTTON", AppResStringsObjects.OKButtonText.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 375, 342, 75, 23, MainObjects.hWndMainWindow, (HMENU)ID_OKBTN, MainObjects.hInst, 0);

	if (!ButtonObjects.hOKBtn)
	{
		MessageBoxW(NULL, L"Failed to create the OK Button", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}

	SendMessageW(ButtonObjects.hOKBtn, WM_SETFONT, (WPARAM)MainObjects.hfDefault, MAKELPARAM(TRUE, 0));
	return 0;
}

// Initialize Changelog Button
BOOL ButtonGUI::InitChangelogBtn()
{
	ButtonObjects.hChangelogBtn = CreateWindowExW(NULL, L"BUTTON", AppResStringsObjects.ChangelogButtonText.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 294, 342, 75, 23, MainObjects.hWndMainWindow, (HMENU)ID_CHANGELOGBTN, MainObjects.hInst, 0);

	if (!ButtonObjects.hChangelogBtn)
	{
		MessageBoxW(NULL, L"Failed to create the Changelog Button", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 1;
	}

	SendMessageW(ButtonObjects.hChangelogBtn, WM_SETFONT, (WPARAM)MainObjects.hfDefault, MAKELPARAM(TRUE, 0));
	return 0;
}