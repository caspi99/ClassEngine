#include "ModuleEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include <GL/glew.h>
#include <string>

ModuleEditor::ModuleEditor(){
	editorWindowShow = true;
	windowMode = 1;
}

ModuleEditor::~ModuleEditor(){

}

bool ModuleEditor::Init(){
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        // IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
	ImGui_ImplOpenGL3_Init();

	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
		maxWidth = displayMode.w;
		maxHeight = displayMode.h;
	}

	return true;
}

update_status ModuleEditor::GeneralMenu() {
	update_status status = UPDATE_CONTINUE;
	if(ImGui::BeginMenu("Menu")) {
		if (ImGui::BeginMenu("About...")) {
			ImGui::Text("Name of the engine: %s", TITLE);
			ImGui::Text("A game engine built in C++ for educational purposes.");
			ImGui::Text("Name of the Author: Andreu Castano");
			ImGui::Separator();
			ImGui::Text("Libraries used");
			ImGui::Text("Glew %s", glewGetString(GLEW_VERSION));
			SDL_version sdlVersion;
			SDL_GetVersion(&sdlVersion);

			ImGui::Text("SDL Version: %d.%d.%d", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
			ImGui::Text("OpenGL Version: %.6s", glGetString(GL_VERSION));
			ImGui::Text("GLSL Version: %.5s", glGetString(GL_SHADING_LANGUAGE_VERSION));
			ImGui::Text("ImGui Version: %s", IMGUI_VERSION);
			ImGui::Text("DirectXTex Version: oct2024");
			ImGui::Text("MathGeoLib Version: 1.5");
			ImGui::Text("TinyGLTF Version: 2.9.3");
			ImGui::Separator();
			ImGui::Text("License: MIT License");
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Engine Github")) system("start https://github.com/caspi99/ClassEngine");
		ImGui::Checkbox("Editor Window", &editorWindowShow);
		if (ImGui::MenuItem("Quit", "Alt+F4")) {
			status = UPDATE_STOP;
		}
		ImGui::EndMenu();
	}
	return status;
}

void ModuleEditor::UpdateFPS() {
	float deltaTimeInSeconds = deltaTime / 1000.0f;
	if (deltaTimeInSeconds > 0.0f) {
		fps = 1.0f / deltaTimeInSeconds;
	}

	fpsHistory[fpsHistoryIndex % FPS_HISTORY_SIZE] = fps;
	fpsHistoryIndex++;
}

void ModuleEditor::ConfigMenu() {
	if (ImGui::BeginMenu("Config Menu")) {
		UpdateFPS();
		ImGui::Text("FPS: %.2f", fps);
		ImGui::PlotHistogram("##framerate", &fpsHistory[0], 100, 0, "", 0.0f, 1500.0f, ImVec2(310, 100));

		ImGui::Separator();
		ImGui::Text("CPUs: %d (Cache: %d bytes)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

		ImGui::Text("GPU: %s", glGetString(GL_RENDERER));
		const GLubyte* vendor = glGetString(GL_VENDOR);
		//This only works for NVIDIA GPUs
		if (vendor != nullptr && std::string(reinterpret_cast<const char*>(vendor)) == "NVIDIA Corporation") {
			int total_vram = 0;
			int free_vram = 0;
			glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_vram);
			glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &free_vram);

			ImGui::Text("Total VRAM: %d MB", total_vram / 1024);
			ImGui::Text("Available VRAM: %d MB", free_vram / 1024);
			ImGui::Text("Used VRAM: %d MB", (total_vram - free_vram) / 1024);
		}
		else ImGui::Text("Not supporting memory consumption for %s", vendor);

		ImGui::Separator();

		const char* windowModes[] = { "Fullscreen", "Windowed without borders", "Windowed" };

		if (ImGui::Combo("Window", &windowMode, windowModes, IM_ARRAYSIZE(windowModes))) {
			App->GetWindow()->ChangeWindowMode(windowMode);
		}
		App->GetWindow()->RenderResolutionSelector();
		App->GetTexture()->setTextConf();

		ImGui::EndMenu();
	}
}

void ModuleEditor::LogConsole() {
	const ImGuiIO& io = ImGui::GetIO();
	const float consoleHeight = io.DisplaySize.y * 0.20f;
	const float mainWindowHeight = io.DisplaySize.y - consoleHeight;
	ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - consoleHeight));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, consoleHeight));

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("Clear Console")) {
		for (const char* message : App->logMessages) {
			free(const_cast<char*>(message));
		}
		App->logMessages.clear();
	}

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);
	ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
	ImGui::PushAllowKeyboardFocus(false);

	for (const char* message : App->logMessages) {
		if (strstr(message, "error")) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message);
		}
		else if (strstr(message, "warning")) {
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), message);
		}
		else {
			ImGui::TextUnformatted(message);
		}
	}

	ImGui::PopAllowKeyboardFocus();
	ImGui::PopTextWrapPos();
	ImGui::EndChild();
	ImGui::End();
}

update_status ModuleEditor::PreUpdate(){
	update_status quitStatus = UPDATE_CONTINUE;
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once); 
	if (ImGui::BeginMainMenuBar()) {
		quitStatus = GeneralMenu();
		if (editorWindowShow) ConfigMenu();
		
		App->GetModel()->modelProperties();
		App->GetRenderExercise()->Lightconf();
		ImGui::EndMainMenuBar();
	}

	LogConsole();

	return quitStatus;
}

update_status ModuleEditor::Update(){
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(){
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}