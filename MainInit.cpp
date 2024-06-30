#include "stdafx.h"
#include "MainInit.h"
#include "MainGUI.h"
#include "ChangelogGUI.h"
#include "ResourceLoader.h"

int MainInit::Init(HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow)
{
	BOOL ret = 0;
	if (Grass7API::Check::isGrass7() == 0) {
		MainObjects.hInst = hInstance;

		// Call the function to load the bitmaps
		ResourceLoader::LoadBitmaps();

		// Load strings
		ResourceLoader::LoadStrings();

		// Register window classes
		ret = MainGUI::Register();
		if (ret != 0) {
			return ret;
		}

		ret = ChangelogGUI::Register();
		if (ret != 0) {
			return ret;
		}

		ret = MainGUI::Init();
		if (ret != 0) {
			return ret;
		}

		ret = ChangelogGUI::Init();
		if (ret != 0) {
			return ret;
		}

		MSG msg;
		while (GetMessageW(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return ret;
}