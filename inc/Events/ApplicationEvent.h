#pragma once

#include "Events/Event.h"

namespace Events
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Close";
			return ss.str();
		}
	};

	class WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(unsigned int a_width, unsigned int a_height)
		: m_width(a_width), m_height(a_height)
		{}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize Event: " << m_width <<", " << m_height << "\n";
			return ss.str();
		}
	private:
		unsigned int m_width, m_height;
	};

	class WindowScrollEvent : public Event
	{
	public:
		WindowScrollEvent(double a_xOffset, double a_yOffset)
			: m_xOffset(a_xOffset), m_yOffset(a_yOffset)
		{}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse)

			std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Scroll Event: " << m_xOffset << ", " << m_yOffset << "\n";
			return ss.str();
		}
		double m_xOffset, m_yOffset;
	};
}