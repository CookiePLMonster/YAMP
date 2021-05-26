#include "YAMPGeneral.h"

#include <ShlObj.h>
#include <filesystem>
#include "wil/resource.h"

YAMPGeneral gGeneral;

std::filesystem::path YAMPGeneral::GetLocalAppDataPath() const
{
	std::filesystem::path result;

	wil::unique_cotaskmem_string str;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr, str.addressof())))
	{
		result.assign(str.get());
	}
	return result;
}
