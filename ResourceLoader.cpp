#include "stdafx.h"
#include "ResourceLoader.h"

void ResourceLoader::LoadBitmaps()
{
	Grass7API::Branding::LoadOSBrandingImage(BitmapObjects.hBanner);
	BitmapObjects.hBanner = Grass7API::Paint::ReplaceColor(BitmapObjects.hBanner, RGB(255, 255, 255), RGB(240, 240, 240), NULL);
}

// Load strings
void ResourceLoader::LoadStrings()
{
	// Load branding string
	Grass7API::Branding::LoadOSBrandingString(MainObjects.szBranding, L"%WINDOWS_GENERIC%");

	// Init variables
	std::wstring OKButtonText(MAX_PATH, 0);
	std::wstring ChangelogButtonText(MAX_PATH, 0);
	std::wstring ErrorTitleText(MAX_PATH, 0);

	// Button strings
	OKButtonText.resize(LoadStringW(MainObjects.hInst, IDS_OKBTN, &OKButtonText[0], (int)OKButtonText.size()));
	ChangelogButtonText.resize(LoadStringW(MainObjects.hInst, IDS_CHANGELOGBTN, &ChangelogButtonText[0], (int)ChangelogButtonText.size()));
	ErrorTitleText.resize(LoadStringW(MainObjects.hInst, IDS_ERRORTITLE, &ErrorTitleText[0], (int)ErrorTitleText.size()));

	// Set loaded wstrings
	AppResStringsObjects.OKButtonText = OKButtonText;
	AppResStringsObjects.ChangelogButtonText = ChangelogButtonText;
	AppResStringsObjects.ErrorTitleText = ErrorTitleText;
}