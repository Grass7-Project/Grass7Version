#include "stdafx.h"
#include "ResourceLoader.h"

void ResourceLoader::LoadBitmaps()
{
	Grass7API::Branding::LoadOSBrandingImage(BitmapObjects.hBanner);
	HDC hdc = nullptr;
	BitmapObjects.hBanner = Grass7API::Paint::ReplaceColor(BitmapObjects.hBanner, RGB(255, 255, 255), RGB(240, 240, 240), hdc);
}

void ResourceLoader::LoadStrings()
{
	InternalLoadBrandingStrings();

	// Button strings
	InternalLoadString(AppResStringsObjects.OKButtonText, IDS_OKBTN);
	InternalLoadString(AppResStringsObjects.ChangelogButtonText, IDS_CHANGELOGBTN);
	InternalLoadString(AppResStringsObjects.ErrorTitleText, IDS_ERRORTITLE);
}

void ResourceLoader::InternalLoadString(std::wstring &strReturn, UINT uID)
{
	std::wstring strTemp(MAX_PATH, 0);
	strTemp.resize((size_t)LoadStringW(MainObjects.hInst, uID, &strTemp[0], (int)strTemp.size()));
	strReturn = strTemp;
	strTemp.clear();
}

void ResourceLoader::InternalLoadBrandingStrings()
{
	PWSTR szGenericBranding = nullptr;
	PWSTR szCopyrightBranding = nullptr;
	PWSTR szShortBranding = nullptr;

	HMODULE hBrandingMod = LoadLibraryExW(L"winbrand.dll", nullptr, NULL);

	Grass7API::Branding::LoadOSBrandingString(hBrandingMod, szGenericBranding, L"%WINDOWS_GENERIC%");
	Grass7API::Branding::LoadOSBrandingString(hBrandingMod, szCopyrightBranding, L"%WINDOWS_COPYRIGHT%");
	Grass7API::Branding::LoadOSBrandingString(hBrandingMod, szShortBranding, L"%WINDOWS_SHORT%");

	BrandingStringsObjects.GenericBrandingText.append(szGenericBranding);
	BrandingStringsObjects.CopyrightBrandingText.append(szCopyrightBranding);
	BrandingStringsObjects.ShortBrandingText.append(szShortBranding);

	GlobalFree((HGLOBAL)szGenericBranding);
	GlobalFree((HGLOBAL)szCopyrightBranding);
	GlobalFree((HGLOBAL)szShortBranding);
	FreeLibrary(hBrandingMod);
}