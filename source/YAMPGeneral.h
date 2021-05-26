#pragma once

#include <filesystem>

// TODO: Move to YAMP namespace maybe?
class YAMPGeneral
{
public:
	const auto& GetDataPath() const { return m_userDataPath; }

	template<typename... Args>
	void SetDataPath(Args&&... args)
	{
		// TODO: Allow for portable mode
		m_userDataPath = GetLocalAppDataPath();
		(m_userDataPath.append(args), ...);
	}

private:
	std::filesystem::path GetLocalAppDataPath() const;

	std::filesystem::path m_userDataPath;
};

extern YAMPGeneral gGeneral;