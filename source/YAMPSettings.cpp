#include "YAMPSettings.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string_view>

static constexpr std::string_view INI_FILE_NAME = "settings.ini";
static constexpr uint32_t SETTINGS_VERSION = 1;

static float GetPrivateProfileFloatW(LPCWSTR lpAppName, LPCWSTR lpKeyName, float fDefault, LPCWSTR lpFileName)
{
	wchar_t buf[32];
	GetPrivateProfileStringW(lpAppName, lpKeyName, nullptr, buf, std::size(buf), lpFileName);
	float val = _wtof(buf);
	if (val != 0.0f)
	{
		return val;
	}
	return fDefault;
}

static void WritePrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, int nValue, LPCWSTR lpFileName)
{
	wchar_t buf[32];
	swprintf_s(buf, L"%d", nValue);
	WritePrivateProfileStringW(lpAppName, lpKeyName, buf, lpFileName);
}

static void WritePrivateProfileFloatW(LPCWSTR lpAppName, LPCWSTR lpKeyName, float fValue, LPCWSTR lpFileName)
{
	wchar_t buf[32];
	swprintf_s(buf, L"%g", fValue);
	WritePrivateProfileStringW(lpAppName, lpKeyName, buf, lpFileName);
}

void YAMPSettings::LoadSettings(const std::filesystem::path& dirPath)
{
	const std::filesystem::path iniPath = dirPath / std::filesystem::u8path(INI_FILE_NAME);

	{
		const wchar_t* SECTION_NAME = L"General";
		if (int version = GetPrivateProfileIntW(SECTION_NAME, L"Version", 0, iniPath.c_str()); version != SETTINGS_VERSION)
		{
			return;
		}
	}

	{
		const wchar_t* SECTION_NAME = L"Graphics";
		int resX = GetPrivateProfileIntW(SECTION_NAME, L"ResolutionX", 0, iniPath.c_str());
		int resY = GetPrivateProfileIntW(SECTION_NAME, L"ResolutionY", 0, iniPath.c_str());
		if (resX != 0 && resY != 0)
		{
			m_resX = resX;
			m_resY = resY;
		}

		float refRate = GetPrivateProfileFloatW(SECTION_NAME, L"RefreshRate", 0.0f, iniPath.c_str());
		if (refRate != 0.0f)
		{
			m_refreshRate = refRate;
		}

		m_fullscreen = GetPrivateProfileIntW(SECTION_NAME, L"Fullscreen", 0, iniPath.c_str()) != 0;
	}

	{
		const wchar_t* SECTION_NAME = L"VF5FS";
		m_arcadeMode = GetPrivateProfileIntW(SECTION_NAME, L"ArcadeMode", 0, iniPath.c_str()) != 0;
		m_circleConfirm = GetPrivateProfileIntW(SECTION_NAME, L"CircleConfirm", 0, iniPath.c_str()) != 0;
		m_language = GetPrivateProfileIntW(SECTION_NAME, L"Language", 1, iniPath.c_str());
	}
}

void YAMPSettings::SaveSettings(const std::filesystem::path& dirPath)
{
	const std::filesystem::path iniPath = dirPath / std::filesystem::u8path(INI_FILE_NAME);

	{
		const wchar_t* SECTION_NAME = L"General";
		WritePrivateProfileIntW(SECTION_NAME, L"Version", SETTINGS_VERSION, iniPath.c_str());
	}

	{
		const wchar_t* SECTION_NAME = L"Graphics";
		WritePrivateProfileIntW(SECTION_NAME, L"ResolutionX", m_resX, iniPath.c_str());
		WritePrivateProfileIntW(SECTION_NAME, L"ResolutionY", m_resY, iniPath.c_str());
	
		WritePrivateProfileFloatW(SECTION_NAME, L"RefreshRate", m_refreshRate, iniPath.c_str());

		WritePrivateProfileIntW(SECTION_NAME, L"Fullscreen", m_fullscreen, iniPath.c_str());
	}

	{
		const wchar_t* SECTION_NAME = L"VF5FS";
		WritePrivateProfileIntW(SECTION_NAME, L"ArcadeMode", m_arcadeMode, iniPath.c_str());
		WritePrivateProfileIntW(SECTION_NAME, L"CircleConfirm", m_circleConfirm, iniPath.c_str());
		WritePrivateProfileIntW(SECTION_NAME, L"Language", m_language, iniPath.c_str());
	}
}
