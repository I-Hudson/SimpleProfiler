#pragma once

#include "imgui.h"

#include <string>
#include <map>

namespace Core
{
	namespace UI
	{
		class UIPanel
		{
		public:
			UIPanel() { }
			UIPanel(UIPanel* parent, const char* panelName, const float& xPos, const float& yPos, const float& width, const float& height);
			UIPanel(UIPanel* parent, const char* panelName, const ImVec2& position, const ImVec2& size);

			~UIPanel() { }

			void SetShowState(const bool& newState);

			bool PositionWithinPanel(ImVec2 position);

			virtual void Update() = 0;

		protected:
			void PreProcessUpdate();

		protected:

			std::string m_panelName;
			UIPanel* m_parent;
			ImVec2 m_position;
			ImVec2 m_size;

			bool m_show;

			// Store a map of UIPanel pointers as the app will own the object, this will just reference it, not own it.
			std::map<std::string, UIPanel*> m_chilren;
		};
	}
}
