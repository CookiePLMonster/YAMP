#include "YAMPUserInterface.h"

#include "imgui/imgui.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <algorithm>

void YAMPUserInterface::Draw()
{
	const ImVec2& displaySize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos({ displaySize.x / 2.0f, displaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 600.0f, 600.0f }, ImGuiCond_Once);

	// Main YAME window
	static bool yame_settings_open = true;
	{
		static bool keyDown = false;
		if (GetAsyncKeyState(VK_F1) & 0x8000)
		{
			if (!keyDown)
			{
				keyDown = true;
				yame_settings_open = !yame_settings_open;
			}
		}
		else
		{
			keyDown = false;
		}
	}
	if (!yame_settings_open)
	{
		return;
	}

	if (ImGui::Begin("YAMP Settings", &yame_settings_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		DrawGraphics();

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

void YAMPUserInterface::DrawGraphics()
{
	if (!ImGui::CollapsingHeader("Graphics"))
		return;

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
			if (m_currentResolutionIndex > 0) m_currentResolutionIndex--;
			else m_currentResolutionIndex = m_resolutions.size() - 1;
		}
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##resolutions##r", ImGuiDir_Right))
		{
			if (++m_currentResolutionIndex >= m_resolutions.size())
				m_currentResolutionIndex = 0;
		}
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
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
			if (m_currentRefRateIndex > 0) m_currentRefRateIndex--;
			else m_currentRefRateIndex = currentRes.refreshRates.size() - 1;
		}
		ImGui::SameLine(0, spacing);
		if (ImGui::ArrowButton("##refrates##r", ImGuiDir_Right))
		{
			if (++m_currentRefRateIndex >= currentRes.refreshRates.size())
				m_currentRefRateIndex = 0;
		}
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text("Refresh Rate");
	}
}
