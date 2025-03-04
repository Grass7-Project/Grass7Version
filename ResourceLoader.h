#pragma once

class ResourceLoader {
public:
	static void LoadBitmaps();
	static void LoadStrings();
private:
	static void InternalLoadString(std::wstring &strReturn, UINT uID);
	static void InternalLoadBrandingStrings();
};