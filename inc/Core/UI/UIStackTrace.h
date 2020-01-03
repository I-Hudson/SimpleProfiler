#pragma once

#include "Core/UI/UIPanel.h"
#include "Core/Tree.h"

#include "Events/ApplicationEvent.h"

namespace Core
{
	namespace UI
	{
		class UIStackTrace : public UIPanel
		{
		public:
			UIStackTrace() { }
			UIStackTrace(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height);
			UIStackTrace(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size);

			virtual void Update() override;

			bool EventProfileResultSelected(Events::AppProfileResultSelectedEvent& event);
			bool EventScroll(Events::WindowScrollEvent& event);

		private:
			void DrawTreeNode(TreeNode* node);

			ImVec2 NonClipedWindowSize();

		private:
			TreeNode* m_displayedTrackNode;

			int m_itemsDrawn;
			float m_scrollY;
			float m_windowSizeY;
			float m_scrollYBuffer = 90.0f;
		};
	}
}