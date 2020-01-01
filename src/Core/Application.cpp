#include "Core/Application.h"

namespace Core
{
	Application::Application()
	{
		PROFILE_FUNCTION();
	}

	Application::~Application()
	{
		PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		PROFILE_FUNCTION();

		OnCreateInternal();
		OnCreate();

		do
		{		
			glClear(GL_COLOR_BUFFER_BIT);	
			m_isRunning = !glfwWindowShouldClose(m_window);
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			OnUpdate();
			OnUpdateInternal();
			
			OnDraw();
			OnDrawInternal();

		} while (m_isRunning);

		OnDestroy();
		OnDestroyInternal();
	}

	bool Application::GetKeyDown(int key)
	{
		PROFILE_FUNCTION();
		return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	void Application::ShowInGUIDemo(bool state)
	{
		PROFILE_FUNCTION();
		m_showImGUIDemo = state;
	}

	void Application::OnCreateInternal()
	{
		PROFILE_FUNCTION();

		if (glfwInit())
		{
			//ErrorCode
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		m_window = glfwCreateWindow(1920, 1080, "C++ Profiler", nullptr, nullptr);
		if (m_window == nullptr)
		{
			//error code
		}
		glfwMakeContextCurrent(m_window);

		if (gladLoadGL())
		{
			//error code
		}

		ImGUICreate();
	}

	void Application::OnUpdateInternal()
	{
		PROFILE_FUNCTION();
		if (m_showImGUIDemo)
		{
			ImGui::ShowDemoWindow(&m_showImGUIDemo);
		}
	}

	void Application::OnDrawInternal()
	{
		PROFILE_FUNCTION();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Application::OnDestroyInternal()
	{
		PROFILE_FUNCTION();

		ImGUIDestroy();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Application::ImGUICreate()
	{
		PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 400");

		ImGui::StyleColorsDark();
	}

	void Application::ImGUIDestroy()
	{
		PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}