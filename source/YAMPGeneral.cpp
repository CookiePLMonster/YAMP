#include "YAMPGeneral.h"

#include <ShlObj.h>
#include <filesystem>
#include "wil/resource.h"

YAMPGeneral gGeneral;

void YAMPGeneral::LoadSettings()
{
	// TODO: Make per-game settings somehow
	m_settings = std::make_unique<YAMPSettings>();
	m_settings->LoadSettings(GetDataPath());
}

std::filesystem::path YAMPGeneral::GetLocalAppDataPath() const
{
	std::filesystem::path result;

	wil::unique_cotaskmem_string str;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, str.addressof())))
	{
		result.assign(str.get());
	}
	return result;
}
