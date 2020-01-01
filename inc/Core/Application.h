#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Profile/Instrumentor.h"
#include "Profile/ProfilerDataProcessor.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

struct GLFWwindow;

	namespace Core
{
	//event callback
	using EventCallbackFn = std::function<void(Events::Event&)>;

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		bool GetKeyDown(int key);

		void static FireEvent(Events::Event& event) { EventCallback(event); }
		void SetEventCallback(const EventCallbackFn& aCallback) { EventCallback = aCallback; }


	protected:
		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;
		virtual void OnDestroy() = 0;

		virtual void OnEvent(Events::Event& event) = 0;

		void ShowInGUIDemo(bool state);
		GLFWwindow* GetWindow() const { return m_window; }

	private:
		void OnCreateInternal();
		void OnUpdateInternal();
		void OnDrawInternal();
		void OnDestroyInternal();

		void OnEventInternal(Events::Event& event);

		void ImGUICreate();
		void ImGUIDestroy();

		//event callback
		static EventCallbackFn EventCallback;

		bool m_isRunning;
		bool m_showImGUIDemo;
		GLFWwindow* m_window;
	};
}