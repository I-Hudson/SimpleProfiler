#pragma once
#include "Core/Application.h"
#include "Core/Tree.h"
#include "Core/UI/UIIncludes.h"

#include "imgui_internal.h"

class ProfileApp : public Core::Application
{
public:
	ProfileApp();
	~ProfileApp();

protected:

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
	virtual void OnDestroy() override;

	virtual void OnEvent(Events::Event& event) override;

private:
	bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);

	void DrawProfileResult(const Core::TreeNode* node);

	bool EventScroll(Events::WindowScrollEvent& event);

	Core::UI::UITimeline m_timeline;
	Core::UI::UIStackTrace m_stackTrace;

	long long m_startTime;
	long long m_endTime;

	uint32_t m_constEndTime;

	Core::Tree m_tree;
};
