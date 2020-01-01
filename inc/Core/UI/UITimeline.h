#pragma once

#include "Core/UI/UIPanel.h"
#include "Core/Tree.h"
#include "Events/ApplicationEvent.h"

namespace Core
{
	namespace UI
	{
		class UITimeline : public UIPanel
		{
		public:
			UITimeline() { }
			UITimeline(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height);
			UITimeline(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size);

			~UITimeline() { }

			void SetTree(Tree* tree);
			void Update() override;

			bool EventScroll(Events::WindowScrollEvent& event);

		private:
			void DrawProfileElement(TreeNode& node);

		private:
			float m_timelineStart;
			float m_timelineCurrent;
			float m_timelineEnd;

			Tree* m_tree;

			float m_timelineAbsEnd;
			float m_contentWidth;
		};
	}
}