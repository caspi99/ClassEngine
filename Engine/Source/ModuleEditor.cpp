#include "ModuleEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "Application.h"

ModuleEditor::ModuleEditor(){
	editorWindowShow = false;
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

update_status ModuleEditor::PreUpdate(){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	update_status quitStatus = GeneralMenu();

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