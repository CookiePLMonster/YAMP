#include "YAMPUserInterface.h"

#include "YAMPGeneral.h"

#include "imgui/imgui.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <algorithm>

static const ImVec4 WARNING_COLOUR { 1.000f, 1.000f, 0.000f, 1.000f };

// "Custom" ImGui wrappers
namespace ImGuiCustom
{
	bool ButtonToggleable(const char* label, const bool enabled, const ImVec2& size = { 0, 0 })
	{
		bool result = false;
		if (!enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		result = ImGui::Button(label, size);

		if (!enabled)
		{
			ImGui::PopStyleVar();
		}
		return enabled ? result : false;
	}

}

#define STRINGIZE(s) STRINGIZE2(s)
#define STRINGIZE2(s) #s

#define WIDEN(x) WIDEN2(x)
#define WIDEN2(x) L ##x

void YAMPUserInterface::Draw()
{
	if (gGeneral.GetSettings()->m_buildLastShowedDisclaimer < rsc_RevisionID)
	{
		DrawDisclaimer();
	}

	if (!ProcessF1Key())
	{
		return;
	}

	const ImVec2& displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos({ displaySize.x / 2.0f, displaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 600, 600 }, ImGuiCond_Once);

	if (ImGui::Begin("YAMP Settings", &m_settingsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		// Helper variables
		int graphics_id, game_id, debug_id, about_id, controls_id;

		static int selectedTab = 0;
		static int delayedSelectedTab = 0; // For confirmation
		ImGui::BeginChild("##left", { 150, 0 }, true);
		{
			auto settingsSection = [this](const char* label, int index, bool confirm)
			{
				const bool selected = selectedTab == index;
				if (ImGui::Selectable(label, selected) && !selected)
				{
					if (!confirm)
					{
						selectedTab = index;
					}
					else
					{
						delayedSelectedTab = index;
						ImGui::OpenPopup("YAMP Settings##save");
					}
				}
				return index;
			};

			int index = 0;
			game_id = settingsSection("Game", index++, m_pageModified);
			graphics_id = settingsSection("Graphics", index++, m_pageModified);
			controls_id = settingsSection("Controls", index++, m_pageModified);
			debug_id = settingsSection("Debug", index++, m_pageModified);
			about_id = settingsSection("About", index++, m_pageModified);

			if (DrawSettingsConfirmation())
			{
				selectedTab = delayedSelectedTab;
			}

			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::BeginGroup();
		bool drawButtons = selectedTab != about_id && selectedTab != controls_id;

		float rightPanelHeight = 0.0f;
		if (drawButtons)
		{
			rightPanelHeight -= ImGui::GetFrameHeightWithSpacing();
			if (m_showRestartWarning)
			{
				rightPanelHeight -= ImGui::GetTextLineHeightWithSpacing() + ImGui::GetTextLineHeight();
			}
		}

		ImGui::BeginChild("##right", { 0, rightPanelHeight });
		{
			if (selectedTab == game_id) DrawGame();
			else if (selectedTab == graphics_id) DrawGraphics();
			else if (selectedTab == controls_id) DrawControls();
			else if (selectedTab == debug_id) DrawDebug();
			else if (selectedTab == about_id) DrawAbout();
		}
		ImGui::EndChild();

		if (drawButtons)
		{
			if (m_showRestartWarning)
			{
				ImGui::PushTextWrapPos();
				ImGui::TextColored(WARNING_COLOUR, "YAMP needs to be restarted for the new settings to take effect.");
				ImGui::PopTextWrapPos();
			}

			if (ImGuiCustom::ButtonToggleable("Apply", m_pageModified))
			{
				ApplySettings();
			}

			ImGui::SameLine();
			if (ImGuiCustom::ButtonToggleable("Cancel", m_pageModified))
			{
				DiscardSettings();
			}
		}
		ImGui::EndGroup();

	}
	ImGui::End();
}

void YAMPUserInterface::AddResolution(uint32_t width, uint32_t height, float refreshRate)
{
	auto it = std::lower_bound(m_resolutions.begin(), m_resolutions.end(), std::make_pair(width, height), [](const Resolution& res, const auto& curr) {
		return std::make_pair(res.width, res.height) < curr;
		});
	if (it != m_resolutions.end() && it->width == width && it->height == height)
	{
	}
	else
	{
		char res[64];
		sprintf_s(res, "%u x %u", width, height);
		it = m_resolutions.insert(it, { width, height, res });
	}

	char refRate[64];
	sprintf_s(refRate, "%.2f Hz", refreshRate);
	it->refreshRates.push_back({ refreshRate, refRate });
}

void YAMPUserInterface::GetDefaultsFromSettings()
{
	const YAMPSettings* settings = gGeneral.GetSettings();

	auto res = std::find_if(m_resolutions.begin(), m_resolutions.end(), [settings](const auto& e) {
		return e.width == settings->m_resX && e.height == settings->m_resY;
		});
	if (res != m_resolutions.end())
	{
		m_currentResolutionIndex = std::distance(m_resolutions.begin(), res);
		auto refRate = std::find_if(res->refreshRates.begin(), res->refreshRates.end(), [settings](const auto& e) {
			return e.refreshRate == settings->m_refreshRate;
			});

		if (refRate != res->refreshRates.end())
		{
			m_currentRefRateIndex = std::distance(res->refreshRates.begin(), refRate);
		}
	}

	m_currentFullscreen = settings->m_fullscreen;
	m_enableFpsCap = settings->m_enableFpsCap;

	m_arcadeMode = settings->m_arcadeMode;
	m_circleConfirm = settings->m_circleConfirm;
	m_language = settings->m_language;

	m_dontApplyPatches = settings->m_dontApplyPatches;
	m_useD3DDebugLayer = settings->m_useD3DDebugLayer;

	// In case non-default Debug options are present, don't nag about the consequences of Debug options for this session
	if (m_dontApplyPatches || m_useD3DDebugLayer)
	{
		m_debugInfoAccepted.reset();
	}
}

void YAMPUserInterface::DrawGraphics()
{
	ImGuiStyle& style = ImGui::GetStyle();
	float w = ImGui::CalcItemWidth();
	float spacing = style.ItemInnerSpacing.x;
	float button_sz = ImGui::GetFrameHeight();

	const auto& currentRes = m_resolutions[m_currentResolutionIndex];
	{
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);

		if (ImGui::BeginCombo("##resolutions", currentRes.displayString.c_str(), ImGuiComboFlags_NoArrowButton))
		{
			size_t index = 0;
			for (const auto& it : m_resolutions)
			{
				const bool isSelected = index == m_currentResolutionIndex;
				if (ImGui::Selectable(it.displayString.c_str(), isSelected))
				{
					m_pageModified = true;
					m_currentResolutionIndex = index;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				++index;
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##resolutions##l", ImGuiDir_Left))
		{
			m_pageModified = true;
			if (m_currentResolutionIndex > 0) m_currentResolutionIndex--;
			else m_currentResolutionIndex = m_resolutions.size() - 1;
		}
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##resolutions##r", ImGuiDir_Right))
		{
			m_pageModified = true;
			if (++m_currentResolutionIndex >= m_resolutions.size())
				m_currentResolutionIndex = 0;
		}
		ImGui::SameLine(0, spacing);
		ImGui::Text("Resolution");
	}
	{
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);

		if (m_currentRefRateIndex >= currentRes.refreshRates.size())
		{
			m_currentRefRateIndex = 0;
		}
		if (ImGui::BeginCombo("##refrates", currentRes.refreshRates[m_currentRefRateIndex].displayString.c_str(), ImGuiComboFlags_NoArrowButton))
		{
			size_t index = 0;
			for (const auto& it : currentRes.refreshRates)
			{
				const bool isSelected = index == m_currentRefRateIndex;
				if (ImGui::Selectable(it.displayString.c_str(), isSelected))
				{
					m_pageModified = true;
					m_currentRefRateIndex = index;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				++index;
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##refrates##l", ImGuiDir_Left))
		{
			m_pageModified = true;
			if (m_currentRefRateIndex > 0) m_currentRefRateIndex--;
			else m_currentRefRateIndex = currentRes.refreshRates.size() - 1;
		}
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##refrates##r", ImGuiDir_Right))
		{
			m_pageModified = true;
			if (++m_currentRefRateIndex >= currentRes.refreshRates.size())
				m_currentRefRateIndex = 0;
		}
		ImGui::SameLine(0, spacing);
		ImGui::Text("Refresh Rate");

	}

	if (ImGui::Checkbox("Fullscreen", &m_currentFullscreen))
	{
		m_pageModified = true;
	}
	if (ImGui::Checkbox("Enable 60 FPS cap", &m_enableFpsCap))
	{
		m_pageModified = true;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Gameplay bugs may occur on uncapped framerates.");
	}
}

// TODO: This will have to be subclassed once more games are added
void YAMPUserInterface::DrawGame()
{
	{
		const char* labels[] = { "Japanese", "English" };
		if (ImGui::BeginCombo("Language", labels[m_language]))
		{
			size_t index = 0;
			for (const auto* label : labels)
			{
				const bool isSelected = index == m_language;
				if (ImGui::Selectable(label, isSelected))
				{
					m_pageModified = true;
					m_language = index;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				++index;
			}

			ImGui::EndCombo();
		}
	}

	{
		const char* labels[] = { "Cross (A)", "Circle (B)" };
		if (ImGui::BeginCombo("Confirmation button", labels[m_circleConfirm]))
		{
			size_t index = 0;
			for (const auto* label : labels)
			{
				const bool isSelected = index == m_circleConfirm;
				if (ImGui::Selectable(label, isSelected))
				{
					m_pageModified = true;
					m_circleConfirm = index != 0;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
				++index;
			}

			ImGui::EndCombo();
		}
	}

	if (ImGui::Checkbox("Arcade Machine Mode", &m_arcadeMode))
	{
		m_pageModified = true;
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("When unchecked, the game runs in console mode.");
	}
}

void YAMPUserInterface::DrawControls()
{
	ImGui::PushItemWidth(ImGui::CalcItemWidth() / 2.0f);

	// TODO: Make these controls customizable
	ImGui::LabelText("Movement", "Arrow Keys / WSAD");
	ImGui::LabelText("P", "K");
	ImGui::LabelText("K", "L");
	ImGui::LabelText("G", "J");
	ImGui::LabelText("P + G", "M");
	ImGui::LabelText("P + K + G", "I");
	ImGui::LabelText("P + K", "U");
	ImGui::LabelText("K + G", "O");
	ImGui::NewLine();
	ImGui::LabelText("Confirm", "Enter");
	ImGui::LabelText("Back", "Escape");
	ImGui::LabelText("Start Game, Select Subcostume, Skip", "F");
	ImGui::LabelText("View Controls, Reset", "Tab");

	ImGui::NewLine();
	ImGui::LabelText("Open YAMP Settings", "F1");

	ImGui::PopItemWidth();
}

void YAMPUserInterface::DrawDebug()
{
	if (m_debugInfoAccepted.has_value())
	{
		bool accept = m_debugInfoAccepted.value();

		ImGui::PushTextWrapPos();
		ImGui::TextColored(WARNING_COLOUR, "WARNING: Do not change any of these options unless you really know what you are doing. "
			"They are intended ONLY for troubleshooting and no support is provided when any debug options are enabled. Use at your own risk.");
		if (!accept)
		{
			ImGui::TextColored(WARNING_COLOUR, "Tick the checkbox below to acknowledge.");
		}
		else
		{
			ImGui::NewLine();
		}
		ImGui::PopTextWrapPos();

		if (ImGui::Checkbox("I understand the consequences", &accept))
		{
			if (accept)
			{
				m_debugInfoAccepted = true;
			}
		}
	}

	if (!m_debugInfoAccepted.value_or(true))
	{
		return;
	}

	if (ImGui::Checkbox("Skip applying patches", &m_dontApplyPatches))
	{
		m_pageModified = true;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("When checked, no patches will be applied to the game DLL.");
	}

	if (ImGui::Checkbox("Use Debug D3D device", &m_useD3DDebugLayer))
	{
		m_pageModified = true;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Enables a debug D3D layer when supported by the system.");
	}
}

void YAMPUserInterface::DrawAbout()
{
	ImGui::PushTextWrapPos();

	// YAMP info
	ImGui::TextUnformatted("Yakuza Arcade Machines Player");
	ImGui::TextUnformatted("Build " STRINGIZE(rsc_RevisionID));
	ImGui::TextUnformatted("Compiled on " __DATE__ " " __TIME__);
	if (ImGui::Button("Check for updates"))
	{
		ShellExecuteW(nullptr, L"open", WIDEN(rsc_UpdateURL), nullptr, nullptr, SW_SHOWNORMAL);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("A GitHub page will open in the web browser.");
	}

	// Game info
	ImGui::Separator();
	ImGui::TextUnformatted("GAME INFORMATION:");

	// TODO: For now it's hardcoded, but it will have to be un-hardcoded later
	ImGui::TextUnformatted("Current arcade: Virtua Fighter 5: Final Showdown");
	ImGui::TextUnformatted("Base game: Yakuza 6");
	ImGui::Text("DLL name: %s", gGeneral.GetDLLName().c_str());
	{
		const time_t timestamp = gGeneral.GetDLLTimestamp();
		tm time;
		if (gmtime_s(&time, &timestamp) == 0)
		{
			char timeStr[64];
			if (strftime(timeStr, std::size(timeStr), "%b %e %Y %T", &time) != 0)
			{
				ImGui::Text("DLL compiled on %s", timeStr);
			}
		}
	}

	// Disclaimers
	ImGui::Separator();
	ImGui::TextUnformatted("ACKNOWLEDGEMENTS:");
	ImGui::TextColored(WARNING_COLOUR, "Yakuza Arcade Machines Player does not redistribute ANY copyrighted files. "
			"You must own an original Steam copy of Yakuza 6: The Song of Life to play games via YAMP. "
			"Pirated game copies WILL NOT receive any support.");

	ImGui::NewLine();
	ImGui::TextUnformatted("All rights to Virtua Fighter 5: Final Showdown belong to SEGA.");

	ImGui::PopTextWrapPos();
}

bool YAMPUserInterface::DrawSettingsConfirmation()
{
	bool result = false;

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("YAMP Settings##save", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Save changes?\n\n");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			ApplySettings();
			ImGui::CloseCurrentPopup();
			result = true;
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
		{
			DiscardSettings();
			ImGui::CloseCurrentPopup();
			result = true;
		}

		ImGui::EndPopup();
	}

	return result;
}

void YAMPUserInterface::DrawDisclaimer()
{
	// Show disclaimer for 20 seconds from the time it's first called
	static const auto displayStartTime = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - displayStartTime) >= std::chrono::seconds(20))
	{
		auto token = gGeneral.GetSettingsUpdateToken();
		auto* settings = token.first;

		settings->m_buildLastShowedDisclaimer = rsc_RevisionID;
	}

	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowPos({ 10.0f, 10.0f }, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.75f);

	if (ImGui::Begin("##disclaimer", nullptr, windowFlags))
	{
		ImGui::TextUnformatted("Welcome to Yakuza Arcade Machines Player (Build " STRINGIZE(rsc_RevisionID) ").");
		ImGui::Separator();

		ImGui::TextColored(WARNING_COLOUR, "DISCLAIMER: Yakuza Arcade Machines Player does not redistribute ANY copyrighted files.\n"
			"You must own an original Steam copy of Yakuza 6: The Song of Life to play games via YAMP.\n"
			"Pirated game copies WILL NOT receive any support.");

		ImGui::NewLine();
		ImGui::TextUnformatted("All rights to Virtua Fighter 5: Final Showdown belong to SEGA.");
		ImGui::Separator();
		ImGui::TextUnformatted("Press "); ImGui::SameLine(0, 0); ImGui::TextColored(WARNING_COLOUR, "F1"); ImGui::SameLine(0, 0); ImGui::TextUnformatted(" to open settings.");
	}
	ImGui::End();
}

bool YAMPUserInterface::ProcessF1Key()
{
	static bool keyDown = false;
	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		if (!keyDown)
		{
			keyDown = true;
			m_settingsOpen = !m_settingsOpen;
		}
	}
	else
	{
		keyDown = false;
	}

	return m_settingsOpen;
}

void YAMPUserInterface::ApplySettings()
{
	auto token = gGeneral.GetSettingsUpdateToken();
	auto* settings = token.first;

	settings->m_resX = m_resolutions[m_currentResolutionIndex].width;
	settings->m_resY = m_resolutions[m_currentResolutionIndex].height;
	settings->m_refreshRate = m_resolutions[m_currentResolutionIndex].refreshRates[m_currentRefRateIndex].refreshRate;
	settings->m_fullscreen = m_currentFullscreen;
	settings->m_enableFpsCap = m_enableFpsCap;

	settings->m_arcadeMode = m_arcadeMode;
	settings->m_circleConfirm = m_circleConfirm;
	settings->m_language = m_language;

	settings->m_dontApplyPatches = m_dontApplyPatches;
	settings->m_useD3DDebugLayer = m_useD3DDebugLayer;

	m_pageModified = false;
	m_showRestartWarning = true;
}

void YAMPUserInterface::DiscardSettings()
{
	GetDefaultsFromSettings();
	m_pageModified = false;
}
