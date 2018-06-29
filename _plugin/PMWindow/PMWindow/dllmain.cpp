// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "stdafx.h"

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <thread>

#include <shellapi.h>
#include <Windows.h>
#include <windowsx.h>
#include <shlobj.h>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl2.h"

#define DLLAPI extern "C" __declspec(dllexport)

static void(*g_logFunctionWide)(const wchar_t *) = NULL;
inline std::wstring utf8_to_utf16(std::string const& src) {
	if (src.empty()) {
		return std::wstring();
	}
	auto const dest_size = ::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, nullptr, 0U);
	std::vector<wchar_t> dest(dest_size, L'\0');
	if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
		return std::wstring();
	}
	return dest.data();
}

static void printW(const wchar_t *text) {
	if (g_logFunctionWide) {
		(*g_logFunctionWide)(text);
	}
}

static void print(const char *utf8string) {
	auto s = utf8_to_utf16(utf8string);
	printW(s.c_str());
}

DLLAPI void set_log_function_wide(void(*f)(const wchar_t *)) {
	g_logFunctionWide = f;
}

static GLFWwindow*  aWindow = nullptr;
static int          aWidth = 640;
static int          aHeight = 480;
static std::string  aTitle = "ParameterWindow";

struct FloatParam {
	bool touch;
	float value;
};
struct BoolParam
{
	bool touch;
	bool value;
};
struct IntParam
{
	bool touch;
	int value;
};

void init() {
	if (aWindow) {
		return;
	}

	glfwInit();
	aWindow = glfwCreateWindow(aWidth, aHeight, aTitle.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(aWindow);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	ImGui_ImplGlfwGL2_Init(aWindow, true);
}

DLLAPI void resetImGui() {
	if (aWindow) {
		ImGui_ImplGlfwGL2_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(aWindow);
		aWindow = nullptr;
	}

	init();
}

DLLAPI void SetWindowVisible(bool visible) {
	init();
	if (visible) {
		glfwShowWindow(aWindow);
	}
	else {
		glfwHideWindow(aWindow);
	}
}

//DLLAPI void CloseParameterWindow() {
//	glfwDestroyWindow(aWindow);
//	aWindow = nullptr;
//}

DLLAPI void beginImGuiDrawing() {
	init();

	glfwPollEvents();

	glfwMakeContextCurrent(aWindow);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplGlfwGL2_NewFrame();
}
DLLAPI void endImGuiDrawing() {
	ImGui::Render();
	ImGui_ImplGlfwGL2_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(aWindow);

	glfwMakeContextCurrent(nullptr);
}

DLLAPI void inputFloat(const char *label, FloatParam *param, float step) {
	param->touch = ImGui::InputFloat(label, &param->value, step);
}
DLLAPI void sliderFloat(const char *label, FloatParam *param, float minValue, float maxValue) {
	param->touch = ImGui::SliderFloat(label, &param->value, minValue, maxValue);
}
DLLAPI void checkbox(const char *label, BoolParam *param) {
	param->touch = ImGui::Checkbox(label, &param->value);
}
DLLAPI void inputInt(const char *label, IntParam *param) {
	param->touch = ImGui::InputInt(label, &param->value);
}

DLLAPI void beginWindow(const char *title, float x, float y, float w, float h) {
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_Appearing);
	ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_Appearing);
	ImGui::SetNextWindowBgAlpha(0.5f);

	ImGui::Begin(title, nullptr);
}


DLLAPI void imguiEnd() {
	ImGui::End();
}

DLLAPI void showDemoWindow() {
	ImGui::ShowDemoWindow();
}

DLLAPI bool treeNode(const char *label, bool defaultOpen) {
	if (defaultOpen) {
		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	}
	return ImGui::TreeNode(label);
}
DLLAPI void treePop() {
	ImGui::TreePop();
}

DLLAPI void text(const char *label) {
	ImGui::Text(label);
}

//DLLAPI void UpdateParameterWindow() {
//	if (aWindow == nullptr) {
//		return;
//	}
//
//	//if (glfwWindowShouldClose(aWindow)) {
//	//	// あとで
//	//	aWindow = nullptr;
//	//	return;
//	//}
//
//	glfwPollEvents();
//
//	glfwMakeContextCurrent(aWindow);
//
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	ImGui_ImplGlfwGL2_NewFrame();
//	
//	ImGui::ShowDemoWindow();
//
//	ImGui::Render();
//	ImGui_ImplGlfwGL2_RenderDrawData(ImGui::GetDrawData());
//
//	glfwSwapBuffers(aWindow);
//
//	glfwMakeContextCurrent(nullptr);
//}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

