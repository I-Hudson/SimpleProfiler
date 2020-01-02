#include "Core/UI/UIStackTrace.h"

namespace Core
{
	namespace UI
	{
		UIStackTrace::UIStackTrace(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height)
			: UIPanel(parent, panelName, xPos, yPos, width, height),
				m_displayedTrackNode(nullptr)
		{
		}

		UIStackTrace::UIStackTrace(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size)
			: UIPanel(parent, panelName, position, size),
				m_displayedTrackNode(nullptr)
		{
		}

		void UIStackTrace::Update()
		{
			PreProcessUpdate();

			ImGui::BeginChild("StackTrace", m_size, true);

			if (m_displayedTrackNode != nullptr)
			{
				if (ImGui::TreeNode(m_displayedTrackNode->GetStringFull().c_str()))
				{

					ImGui::TreePop();
				}
			}

			ImGui::EndChild();
		}

		bool UIStackTrace::EventProfileResultSelected(Events::AppProfileResultSelectedEvent& event)
		{
			m_displayedTrackNode = static_cast<TreeNode*>(event.m_treeNode);

			return false;
		}
	}
}