#pragma once

#include <filesystem>
#include <memory>

#include "wil/resource.h"

#include "YAMPSettings.h"

// TODO: Move to YAMP namespace maybe?
class YAMPGeneral
{
public:
	const auto& GetDataPath() const { return m_userDataPath; }
	const auto* GetSettings() const { return m_settings.get(); }

	template<typename... Args>
	void SetDataPath(Args&&... args)
	{
		// TODO: Allow for portable mode
		m_userDataPath = GetLocalAppDataPath();
		(m_userDataPath.append(args), ...);
	}
	
	void LoadSettings();

	// For use in the UI
	auto GetSettingsUpdateToken()
	{
		// (settings_ptr, apply_token) pair
		// Calls SaveSettings when it goes out of scope
		return std::make_pair(m_settings.get(), wil::scope_exit([this] {
			m_settings->SaveSettings(GetDataPath());
		}));
	}

private:
	std::filesystem::path GetLocalAppDataPath() const;

	std::filesystem::path m_userDataPath;
	std::unique_ptr<YAMPSettings> m_settings;
};

extern YAMPGeneral gGeneral;