#pragma once

#include <optional>
#include <vector>
#include <string>
#include <tuple>

class YAMPUserInterface
{
public:
	YAMPUserInterface() = default;

	void Draw();
	void AddResolution(uint32_t width, uint32_t height, float refreshRate);
	void GetDefaultsFromSettings();

private:
	void DrawGraphics();
	void DrawGame();
	void DrawControls();
	void DrawDebug();
	bool DrawSettingsConfirmation();

	void DrawDisclaimer();

	bool ProcessF1Key();

	void ApplySettings();
	void DiscardSettings();

private:
	struct Resolution
	{
		struct RefreshRate
		{
			float refreshRate;
			std::string displayString;
		};

		uint32_t width, height;
		std::string displayString;
		std::vector<RefreshRate> refreshRates;
	};

	std::vector<Resolution> m_resolutions;

	// Current settings
	// Graphics
	size_t m_currentResolutionIndex = 0;
	size_t m_currentRefRateIndex = 0;
	bool m_currentFullscreen = false;

	// Game settings
	// TODO: Subclass once more games are added
	bool m_arcadeMode = false;
	bool m_circleConfirm = false;
	uint32_t m_language = 1;

	// Debug settings
	bool m_dontApplyPatches = false;
	bool m_useD3DDebugLayer = false;

	// Volatile state
	bool m_settingsOpen = false, m_pageModified = false, m_showRestartWarning = false;
	std::optional<bool> m_debugInfoAccepted { false };
};