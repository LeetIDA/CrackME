#include "gui.h"
#include "xorstr.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void gui::Render() noexcept
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.09f, 0.12f, 1.00f)); // Change button color
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.23f, 0.29f, 1.00f)); // Change button color when hovered
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change button color when active
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.80f, 0.83f, 1.00f)); // Change text color
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.24f, 0.23f, 0.29f, 1.00f)); // Change text color when disabled
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.05f, 0.07f, 1.00f)); // Change window background color
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f)); // Change title background color
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(1.00f, 0.98f, 0.95f, 0.75f)); // Change title background color when collapsed
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.07f, 0.07f, 0.09f, 1.00f)); // Change title background color when active
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f)); // Change scrollbar background color
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.80f, 0.80f, 0.83f, 0.31f)); // Change scrollbar grab color
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change scrollbar grab color when hovered
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f)); // Change scrollbar grab color when active
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.80f, 0.80f, 0.83f, 0.31f)); // Change checkmark color
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.80f, 0.80f, 0.83f, 0.31f)); // Change slider grab color
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f)); // Change slider grab color when active
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.10f, 0.09f, 0.12f, 1.00f)); // Change header color
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change header color when hovered
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f)); // Change header color when active
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change separator color
	ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(0.24f, 0.23f, 0.29f, 1.00f)); // Change separator color when hovered
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change separator color when active
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.00f, 0.00f, 0.00f, 0.00f)); // Change resize grip color
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f)); // Change resize grip color when hovered
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f)); // Change resize grip color when active
	ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.40f, 0.39f, 0.38f, 0.63f)); // Change plot lines color
	ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f)); // Change plot lines color when hovered
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.40f, 0.39f, 0.38f, 0.63f)); // Change plot histogram color
	ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f)); // Change plot histogram color when hovered
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.25f, 1.00f, 0.00f, 0.43f)); // Change text selected background color
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(1.00f, 0.98f, 0.95f, 0.73f)); // Change modal window dim background color
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.20f, 0.20f, 0.25f, 1.00f)); // Background color
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.30f, 1.00f)); // Background color when hovered
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.30f, 0.30f, 0.35f, 1.00f));  // Background color when active

	static char buf[256];
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"CrackME ALPHA",
		&isRunning,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);
	ImGui::Text(XorStr("Key:").c_str());
	ImGui::InputText("", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll);
	if (ImGui::Button("Submit")) {
		if (strcmp(buf, XorStr("cqt+r?9_)*lv0)+ex4rn2fpbh*?w0*50x?b1u?j*bjqv8bem564").c_str()) == 0) {
			MessageBoxA(NULL, "Cracked\nI've Never Found It Hard To Hack Most People.", "CrackME ALPHA", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBoxA(NULL, "Maybe Wars Aren't Meant To Be Won,\nMaybe They're Meant To Be Continuous.", "CrackME ALPHA", MB_OK | MB_ICONERROR);
		}
	}
	ImGui::Text(XorStr("Made With Love By IDA").c_str());

	ImGui::End();
}
