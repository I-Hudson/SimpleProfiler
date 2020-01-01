#include "Core/UI/UIPanel.h"

namespace Core
{
	namespace UI
	{
		UIPanel::UIPanel(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height)
			: m_parent(parent), m_panelName(std::string(panelName)), m_position(ImVec2(xPos, yPos)), m_size(ImVec2(width, height)), m_show(true)
		{

		}

		UIPanel::UIPanel(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size)
			: m_parent(parent), m_panelName(std::string(panelName)), m_position(position), m_size(size), m_show(true)
		{

		}

		void UIPanel::SetShowState(const bool& newState)
		{
			if (!m_chilren.empty())
			{
				for (auto obj : m_chilren)
				{
					obj.second->SetShowState(newState);
				}
			}

			m_show = false;
		}

		void UIPanel::PreProcessUpdate()
		{
			ImGui::SetNextWindowPos(m_position);
			ImGui::SetNextWindowSize(m_size);
		}
	}
}