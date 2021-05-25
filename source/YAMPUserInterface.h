#pragma once

#include <vector>
#include <string>
#include <tuple>

class YAMPUserInterface
{
public:
	YAMPUserInterface() = default;

	void Draw();
	void AddResolution(uint32_t width, uint32_t height, float refreshRate);

private:
	void DrawGraphics();

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
	size_t m_currentResolutionIndex = 0;
	size_t m_currentRefRateIndex = 0;
};