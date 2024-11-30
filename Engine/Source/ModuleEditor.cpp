#include "ModuleEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "Application.h"
#include <GL/glew.h>

ModuleEditor::ModuleEditor(){
	editorWindowShow = true;
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

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->GetContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleEditor::GeneralMenu() {
	ImGui::Begin("Menu");
	if (ImGui::BeginMenu("About...")) {
		ImGui::Text("Name of the engine: %s", TITLE);
		ImGui::Text("This is our super awesome engine");
		ImGui::Text("Name of the Author: Andreu Castano");
		ImGui::Text("Libraries(with versions) used");
		ImGui::Text("Using Glew %s", glewGetString(GLEW_VERSION));
		ImGui::Text("License");
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Engine Github")) system("start https://github.com/caspi99/ClassEngine");
	ImGui::Checkbox("Editor Window", &editorWindowShow);
	if (ImGui::MenuItem("Quit", "Alt+F4")) {
		return UPDATE_STOP;
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

// Function to update FPS
void ModuleEditor::UpdateFPS() {
	if (App->deltaTime > 0.0f) {
		fps = 1.0f / App->deltaTime;
	}

	// Store FPS in history array and calculate the average FPS
	fpsHistory[fpsHistoryIndex % FPS_HISTORY_SIZE] = fps;
	fpsHistoryIndex++;
}

void ModuleEditor::ConfigMenu() {
	ImGui::Begin("Config Menu");

	UpdateFPS();
	ImGui::Text("FPS: %.2f", fps);
	ImGui::PlotHistogram("##framerate", &fpsHistory[0], 100, 0, "", 0.0f, 1500.0f, ImVec2(310, 100));

	ImGui::Separator();
	ImGui::Text("CPUs: %d (Cache: %d bytes)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
	ImGui::Text("System RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

	//This is not working
	int total_vram = 0;
	int free_vram = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_vram);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &free_vram);

	ImGui::Text("Total VRAM: %d MB", total_vram / 1024);
	ImGui::Text("Available VRAM: %d MB", free_vram / 1024);
	ImGui::Text("Used VRAM: %d MB", (total_vram - free_vram) / 1024);

	ImGui::Separator();
	ImGui::Text("GPU: %s", glGetString(GL_RENDERER));

	SDL_version sdlVersion;
	SDL_GetVersion(&sdlVersion);

	ImGui::Text("SDL Version: %d.%d.%d", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	ImGui::Text("OpenGL Version: %.6s", glGetString(GL_VERSION));
	ImGui::Text("GLSL Version: %.5s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	ImGui::End();
}

update_status ModuleEditor::PreUpdate(){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	update_status quitStatus = GeneralMenu();
	if(editorWindowShow) ConfigMenu();

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