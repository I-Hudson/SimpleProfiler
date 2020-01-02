#include "Core/UI/UITimeline.h"

#include "ProfileApp.h"
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
				m_timelineStart(0), m_timelineCurrent(0), m_timelineEnd(0), m_tree(nullptr), m_timelineAbsEnd(0), m_contentWidth(size.x)
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
			DrawTimelineHeader();

			ImGui::SetNextWindowContentWidth(m_contentWidth);
			PreProcessUpdate();
			ImGui::BeginChild(m_panelName.c_str(), m_size, true, ImGuiWindowFlags_HorizontalScrollbar);

			std::cout << "Scroll X: " << ImGui::GetScrollX() << "\n";

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
			m_timelineEnd += (long long)(event.m_yOffset * (m_timelineEnd * 0.5f));

			if (m_timelineEnd < 1)
			{
				m_timelineEnd = 1;
			}
			else if (m_timelineEnd > m_timelineAbsEnd)
			{
				m_timelineEnd = m_timelineAbsEnd;
			}

			m_contentWidth = (m_timelineAbsEnd / m_timelineEnd) * m_size.x;

			return false;
		}

		void UITimeline::DrawTimelineHeader()
		{
			for (size_t i = 0; i < 10; i++)
			{

				double sPos = (m_timelineEnd / 10.0) * (double)i;
				double nPosition = sPos / m_timelineEnd;
				double nSize = m_size.x / 20;

				std::stringstream ss;

				double displayTime = sPos / 1000.0;
				ss << "ms:";
				if (displayTime > 1.0)
				{
					ss << (uint32_t)displayTime;
				}
				else
				{
					ss << displayTime;
				}

				ImGui::SetCursorPosX((float)nPosition * m_size.x);
				ImGui::SetCursorPosY(25);
				if (ImGui::Button(ss.str().c_str(), ImVec2((float)nSize, 25))) { m_timelineEnd = (long long)sPos; }
			}
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
				Events::AppProfileResultSelectedEvent event(static_cast<void*>(&node));
				ProfileApp::FireEvent(event);
			}
		}
	}
}