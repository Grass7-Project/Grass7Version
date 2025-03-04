#include "stdafx.h"
#include "MainInit.h"
#include "MainGUI.h"
#include "ChangelogGUI.h"
#include "ResourceLoader.h"
#include "ExitHandler.h"

int MainInit::Init(HINSTANCE &hInstance)
{
	if (Grass7API::Check::isGrass7() == 0) {
		MainObjects.hInst = hInstance;
		std::atexit(ExitHandler::Cleanup);

		// Call the function to load the bitmaps
		ResourceLoader::LoadBitmaps();

		// Load strings
		ResourceLoader::LoadStrings();

		MainObjects.hModRichEdit = LoadLibraryExW(L"Msftedit.dll", nullptr, NULL);
		if (!MainObjects.hModRichEdit) {
			MessageBoxW(nullptr, L"For some reason, the static bar failed to be created", AppResStringsObjects.ErrorTitleText.c_str(), MB_ICONERROR | MB_OK);
			return 1;
		}

		// Register window classes
		if (MainGUI::Register()) {
			return 1;
		}

		if (ChangelogGUI::Register()) {
			return 1;
		}

		if (ChangelogGUI::Init()) {
			return 1;
		}

		if (MainGUI::Init()) {
			return 1;
		}

		MSG msg;
		while (GetMessageW(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return 0;
}