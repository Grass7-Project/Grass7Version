#pragma once

class MainGUI {
public:
	static BOOL Init();
	static BOOL Register();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void PopulateVersionText();

	int wSizeX,
		wSizeY,
		FirstLine,
		LastLine;

	HWND hWndStaticBar;

	std::wstring szTitle;
	std::wstring RegisteredOwner;
	std::wstring RegisteredOrganization;
	std::vector< std::wstring > VersionText;
};