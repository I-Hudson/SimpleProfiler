#include "Core/UI/UITimeline.h"

#include <iostream>

namespace Core
{
	namespace UI
	{
		UITimeline::UITimeline(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height)
			: UIPanel(parent, panelName, xPos, yPos, width, height),
			m_timelineStart(0.0f), m_timelineCurrent(0.0f), m_timelineEnd(0.0f), m_tree(nullptr), m_timelineAbsEnd(0.0f), m_contentWidth(width)
		{
		}

		UITimeline::UITimeline(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size)
			: UIPanel(parent, panelName, position, size),
				m_timelineStart(0.0f), m_timelineCurrent(0.0f), m_timelineEnd(0.0f), m_tree(nullptr), m_timelineAbsEnd(0.0f), m_contentWidth(size.x)
		{
		}

		void UITimeline::SetTree(Tree* tree)
		{
			if (tree != nullptr)
			{
				m_tree = tree;

				m_timelineEnd = m_tree->GetLastRootNode()->GetMircoEnd();
				m_timelineAbsEnd = m_tree->GetLastRootNode()->GetMircoEnd();
			}
			else
			{
				//@TODO: Error message
			}
		}

		void UITimeline::Update()
		{
			ImGui::SetNextWindowContentWidth(m_contentWidth);
			PreProcessUpdate();
			ImGui::BeginChild(m_panelName.c_str(), m_size, true, ImGuiWindowFlags_HorizontalScrollbar);

			const int rowHeight = 25;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			for (size_t i = 0; i < m_tree->m_rootNodes.size(); ++i)
			{
				DrawProfileElement(*m_tree->m_rootNodes[i]);
			}
			ImGui::PopStyleVar();

			ImGui::EndChild();
		}

		bool UITimeline::EventScroll(Events::WindowScrollEvent& event)
		{
			m_timelineEnd += event.m_yOffset * 250;

			if (m_timelineEnd < 1)
			{
				m_timelineEnd = 0;
			}
			else if (m_timelineEnd > m_timelineAbsEnd)
			{
				m_timelineEnd = m_timelineAbsEnd;
			}

			m_contentWidth = (m_timelineAbsEnd / m_timelineEnd) * m_size.x;

			std::cout << m_timelineEnd << "\n";

			return false;
		}

		void UITimeline::DrawProfileElement(TreeNode& node)
		{
			//@TODO. Draw call check

			for (size_t i = 0; i < node.Children.size(); ++i)
			{
				DrawProfileElement(*node.Children[i]);
			}

			double elementPosition = (double)node.ProfileResult.Start / (double)m_timelineEnd;
			double elementSize = ((double)node.ProfileResult.End - (double)node.ProfileResult.Start) / (double)m_timelineEnd;

			if (elementSize == 0.0)
			{
				return;
			}

			std::stringstream ss;
			ss << node.ProfileResult.Name;
			ss << " ms" << node.ProfileResult.End - node.ProfileResult.Start;

			ImGui::SetCursorPosX((float)elementPosition * m_size.x);
			ImGui::SetCursorPosY((node.y + 1) * 25.0f);
			if (ImGui::Button(ss.str().c_str(), ImVec2((float)elementSize * m_size.x, 25.0f)))
			{
				//@TODO. Send event, profile result has been clicked.
			}
		}
	}
}