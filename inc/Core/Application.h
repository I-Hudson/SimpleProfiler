#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Profile/Instrumentor.h"
#include "Profile/ProfilerDataProcessor.h"

struct GLFWwindow;

	namespace Core
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		bool GetKeyDown(int key);

	protected:
		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;
		virtual void OnDestroy() = 0;

		void ShowInGUIDemo(bool state);
		GLFWwindow* GetWindow() const { return m_window; }

	private:
		void OnCreateInternal();
		void OnUpdateInternal();
		void OnDrawInternal();
		void OnDestroyInternal();

		void ImGUICreate();
		void ImGUIDestroy();

		bool m_isRunning;
		bool m_showImGUIDemo;
		GLFWwindow* m_window;
	};
}