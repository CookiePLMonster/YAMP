#pragma once

#include <array>
#include <filesystem>
#include <memory>

#include <cassert>

#include "wil/resource.h"

#include "YAMPSettings.h"

// TODO: Move
static std::wstring UTF8ToWchar(std::string_view text)
{
	std::wstring result;

	const int count = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
	if ( count != 0 )
	{
		result.resize(count);
		MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), count);
	}

	return result;
}

static std::string WcharToUTF8(std::wstring_view text)
{
	std::string result;

	const int count = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
	if ( count != 0 )
	{
		result.resize(count);
		WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), count, nullptr, nullptr);
	}

	return result;
}

// TODO: Move to YAMP namespace maybe?
class YAMPGeneral
{
public:
	const auto& GetDataPath() const { return m_userDataPath; }
	const auto* GetSettings() const { return m_settings.get(); }
	const auto& GetPressedKeys() const { return m_pressedKeyboardKeys; }
	const auto& GetDLLName() const { return m_dllName; }
	const auto GetDLLTimestamp() const { return m_dllTimestamp; }

	template<typename... Args>
	void SetDataPath(Args&&... args)
	{
		// TODO: Allow for portable mode
		m_userDataPath = GetLocalAppDataPath();
		(m_userDataPath.append(args), ...);
	}
	
	void SetDLLName(std::string name) { m_dllName = std::move(name); }
	void SetDLLTimestamp(uint32_t timestamp) { m_dllTimestamp = timestamp; }

	void SetKeyPressed(uint32_t key, bool pressed)
	{
		assert(key < m_pressedKeyboardKeys.size());
		m_pressedKeyboardKeys[key] = pressed;
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

	std::string m_dllName;
	uint32_t m_dllTimestamp = 0;
	std::filesystem::path m_userDataPath;
	std::unique_ptr<YAMPSettings> m_settings;

	std::array<bool, 256> m_pressedKeyboardKeys;
};

extern YAMPGeneral gGeneral;