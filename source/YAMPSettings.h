#pragma once

#include <filesystem>

class YAMPSettings
{
public:
	void LoadSettings(const std::filesystem::path& dirPath);
	void SaveSettings(const std::filesystem::path& dirPath);

public:
	uint32_t m_resX = 1280;
	uint32_t m_resY = 720;
	float m_refreshRate = 60.0f;
};