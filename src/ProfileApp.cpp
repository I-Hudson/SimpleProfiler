#include "ProfileApp.h"
#include <iostream>
#include <string>
#include <sstream>

#include <chrono>
#include <thread>
#include <future>

#define NEXT_WINDOW(x, y, xSize, ySize) ImGui::SetNextWindowPos(ImVec2(x, y)); 	ImGui::SetNextWindowSize(ImVec2(xSize, ySize));
#define LOG_LINE(x) std::cout << x << "\n";

long long ProfileApp::m_endTime = 0;
uint32_t ProfileApp::m_constEndTime = 0;

float WINDOW_X_SIZE = 1920;
float WINDOW_Y_SIZE = 1080;

float contentWidth = WINDOW_X_SIZE;

#define BIND_EVENT_FUNC(x) (std::bind(&x, this, std::placeholders::_1))

ProfileApp::ProfileApp()
{
	PROFILE_FUNCTION();
}

ProfileApp::~ProfileApp()
{
	PROFILE_FUNCTION();
}


void ProfileApp::OnCreate()
{
	PROFILE_FUNCTION();
	GLFWwindow* window = (GLFWwindow*)ImGui::GetIO().ClipboardUserData;
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
		});

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			Events::WindowResizeEvent event(width, height);
			FireEvent(event);
		});

	Profile::ProfilerDataProcessor data;
	data.Load("2k.json");

	if (data.DataReady())
	{
		m_tree.CreateTree(data.GetProfileResults());
		m_tree.PrintTreeToConsole(nullptr);

		if (m_tree.IsReady())
		{
			m_startTime = 0;		//m_tree.GetFirstRootNode()->GetMircoStart();
			m_endTime = m_tree.GetLastRootNode()->GetMircoEnd();
			m_constEndTime = m_tree.GetLastRootNode()->GetMircoEnd();
		}
	}
}

void ProfileApp::OnUpdate()
{
	PROFILE_FUNCTION();

	if (!m_tree.IsReady())
	{
		return;
	}

	bool open = true;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_X_SIZE, WINDOW_Y_SIZE));
	ImGui::Begin("Window", &open);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { open = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//Splitter(true, 8.0f, &WINDOW_X_SIZE, &WINDOW_X_SIZE, 8, 8, WINDOW_Y_SIZE * 0.25f);

	for (size_t i = 0; i < 10; i++)
	{

		double sPos = ((double)m_endTime / 10.0) * (double)i;
		double nPosition = sPos / (double)m_endTime;
		double nSize = WINDOW_X_SIZE / 20;

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

		ImGui::SetCursorPosX((float)nPosition * WINDOW_X_SIZE);
		ImGui::SetCursorPosY(25);
		if (ImGui::Button(ss.str().c_str(), ImVec2(nSize, 25))) { m_endTime = sPos; }
	}

	ImGui::SetNextWindowContentWidth(contentWidth);
	ImGui::SetNextWindowPos(ImVec2(0, 50));
	ImGui::BeginChild("BarGraph", ImVec2(WINDOW_X_SIZE, WINDOW_Y_SIZE * 0.2f), true, ImGuiWindowFlags_HorizontalScrollbar);

	int maxNumberOfButtons = m_tree.m_rootNodes.size();

	const int rowHeight = 25;

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	for (int i = 0; i < maxNumberOfButtons; i++)
	{
		DrawProfileResult(m_tree.m_rootNodes[i]);
	}
	ImGui::PopStyleVar();

	ImGui::EndChild();

	ImGui::SetNextWindowPos(ImVec2(0, 0.5f * WINDOW_Y_SIZE));
	ImGui::BeginChild("StackTrace", ImVec2(WINDOW_X_SIZE, 0.5f * WINDOW_Y_SIZE), true);

	ImGui::EndChild();

	ImGui::End();

	if (GetKeyDown(GLFW_KEY_E))
	{
		m_endTime = 50.0f;
	}

	if (GetKeyDown(GLFW_KEY_S))
	{
		m_endTime = m_constEndTime;
	}
}

void ProfileApp::OnDraw()
{
	PROFILE_FUNCTION();
}

void ProfileApp::OnDestroy()
{
	PROFILE_FUNCTION();
	m_tree.Destroy();
}

void ProfileApp::OnEvent(Events::Event& event)
{
	Events::EventDispatcher dispatcher(event);

	dispatcher.Dispatch<Events::WindowResizeEvent>(BIND_EVENT_FUNC(ProfileApp::EventWindowResize));
}

bool ProfileApp::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	PROFILE_FUNCTION();

	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

int drawCall = 0;

void ProfileApp::DrawProfileResult(const  Core::TreeNode* node)
{
	if (drawCall > 500)
	{
		std::stringstream ss;
		ss << drawCall;
		ImGui::BeginChild(ss.str().c_str());
		ImGui::EndChild();
		drawCall = 0;
	}

	for (size_t i = 0; i < node->Children.size(); i++)
	{
		DrawProfileResult(node->Children[i]);
	}

	double nPosition = (double)node->ProfileResult.Start / (double)m_endTime;
	double nSize = ((double)node->ProfileResult.End - (double)node->ProfileResult.Start) / (double)m_endTime;

	if (nSize == 0.0)
	{
		return;
	}

	std::stringstream ss;
	ss << node->ProfileResult.Name;
	ss << " ms" << node->ProfileResult.End - node->ProfileResult.Start;

	ImGui::SetCursorPosX((float)nPosition * WINDOW_X_SIZE);
	ImGui::SetCursorPosY((node->y + 1) * 25);
	if (ImGui::Button(ss.str().c_str(), ImVec2(nSize * WINDOW_X_SIZE, 25)))
	{
		std::stringstream ss;
	
		ss << "Id: " << node->ProfileResult.Id << "\n";
		ss << "Start: " << node->ProfileResult.Start << "\n";
		ss << "End: " << node->ProfileResult.End << "\n";
		ss << "ThreadId: " << node->ProfileResult.ThreadID << "\n";
		ss << "Name: " << node->ProfileResult.Name << "\n";
	
		std::cout << ss.str() << "\n";
	}
	drawCall++;
}

bool ProfileApp::EventWindowResize(Events::WindowResizeEvent& event)
{
	return false;
}

void ProfileApp::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	m_endTime += yOffset * 250;

	if (m_endTime < 1)
	{
		m_endTime = 1;
	}

	if (m_endTime > m_constEndTime)
	{
		m_endTime = m_constEndTime;
	}

	contentWidth = ((double)m_constEndTime / (double)m_endTime) * WINDOW_X_SIZE;

	std::cout << m_endTime << "\n";
}