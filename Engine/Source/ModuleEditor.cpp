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

void ModuleEditor::ConfigMenu() {
	ImGui::Begin("Config Menu");
	//Esto es lo mas jodido de ImGui

	ImGui::Text("CPU Cores: %d", SDL_GetCPUCount());
	ImGui::Text("CPU Cache Line Size: %d bytes", SDL_GetCPUCacheLineSize());
	ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

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