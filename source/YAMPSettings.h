#pragma once

#include <filesystem>

class YAMPSettings
{
public:
	void LoadSettings(const std::filesystem::path& dirPath);
	void SaveSettings(const std::filesystem::path& dirPath);

public:
	// Graphics
	uint32_t m_resX = 1280;
	uint32_t m_resY = 720;
	float m_refreshRate = 60.0f;
	bool m_fullscreen = false;
	bool m_enableFpsCap = true;

	// Game
	// TODO: Subclass once more games are added
	bool m_arcadeMode = false;
	bool m_circleConfirm = false;
	uint32_t m_language = 1; // English

	// Debug settings
	bool m_dontApplyPatches = false;
	bool m_useD3DDebugLayer = false;

	// Misc
	uint32_t m_buildLastShowedDisclaimer = 0;
};