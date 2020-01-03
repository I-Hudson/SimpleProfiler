#include "Core/UI/UIStackTrace.h"

#include <iostream>

namespace Core
{
	namespace UI
	{
		UIStackTrace::UIStackTrace(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height)
			: UIPanel(parent, panelName, xPos, yPos, width, height),
				m_displayedTrackNode(nullptr), m_scrollY(0.0f), m_itemsDrawn(0)
		{
		}

		UIStackTrace::UIStackTrace(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size)
			: UIPanel(parent, panelName, position, size),
				m_displayedTrackNode(nullptr), m_scrollY(0.0f), m_itemsDrawn(0)
		{
		}

		void UIStackTrace::Update()
		{
			PreProcessUpdate();

			ImGui::BeginChild("StackTrace", m_size, true);
			ImGui::SetScrollY(m_scrollY);

			if (m_displayedTrackNode != nullptr)
			{
				DrawTreeNode(m_displayedTrackNode);
			}

			ImGui::EndChild();
		}

		bool UIStackTrace::EventProfileResultSelected(Events::AppProfileResultSelectedEvent& event)
		{
			m_displayedTrackNode = static_cast<TreeNode*>(event.m_treeNode);

			return false;
		}

		bool UIStackTrace::EventScroll(Events::WindowScrollEvent& event)
		{
			if (!PositionWithinPanel(ImGui::GetMousePos()))
			{
				return false;
			}

			m_scrollY -= event.m_yOffset * 10;

			ImVec2 nonClipedSize = NonClipedWindowSize();

			if (m_scrollY < 0.1f)
			{
				m_scrollY = 0.1f;
			}
			else if (m_scrollY >= m_size.y)
			{
				m_scrollY = m_size.y;
			}

			std::cout << m_scrollY << "\n";

			return false;
		}

		void UIStackTrace::DrawTreeNode(TreeNode* node)
		{
			int numOfChildren = node->Children.size();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed;
			flags |= numOfChildren == 0 ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;
			if (ImGui::TreeNodeEx(node->GetStringFull().c_str(), flags))
			{
				m_itemsDrawn++;
				for (size_t i = 0; i < numOfChildren; ++i)
				{
					DrawTreeNode(node->Children[i]);
				}
				ImGui::TreePop();
			}
		}

		ImVec2 UIStackTrace::NonClipedWindowSize()
		{
			ImVec2 size = m_size;
			size.y = m_itemsDrawn * ImGui::GetStyle().FramePadding.y;
			return size;
		}
	}
}