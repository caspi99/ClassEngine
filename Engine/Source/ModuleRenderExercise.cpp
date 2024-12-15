#include "ModuleRenderExercise.h"
#include <GL/glew.h>
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"
#include "ImGui/imgui.h"

ModuleRenderExercise::ModuleRenderExercise() {
	program = 0;
	width = 0;
	height = 0;
	ambientIntensity = 0.1f;
	camera = nullptr;
}

ModuleRenderExercise::~ModuleRenderExercise() {

}

void ModuleRenderExercise::Lightconf() {
	if (ImGui::BeginMenu("Phong Menu")) {
		if (ImGui::ColorEdit3("Light Color", &light.color[0])) {
			glUseProgram(program);
			glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, &light.color[0]);
		}

		if (ImGui::SliderFloat3("Light Direction", &light.direction[0], -2.0f, 2.0f)) {
			glUseProgram(program);
			glUniform3fv(glGetUniformLocation(program, "lightDir"), 1, &light.direction[0]);
		}

		if (ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0, 1)) {
			glUseProgram(program);
			glUniform1f(glGetUniformLocation(program, "ambientIntensity"), ambientIntensity);
		}
		
		ImGui::EndMenu();
	}
}

bool ModuleRenderExercise::Init() {
	program = App->GetProgram()->CreateProgram("default_vertex.glsl", "phong_fragment.glsl");
	if (program == 0) {
		LOG("Error: Failed to create shader program");
		return false;
	}

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -10.0f),
		float4x4::RotateZ(0.0f),
		float3(3.f, 3.f, 3.f));

	camera = App->GetCamera();
	camera->PerspectiveCamera(float3(0.0f, 3.0f, -10.0f), pi / 4.0f, 0.1f, 10000.0f);
	camera->LookAt(float3(0.0f, -1.0f, 10.0f));
	UpdateCamera();
	
	App->GetModel()->Load("BakerHouse.gltf");

	projection = camera->GetProjectionMatrix();

	light.color = { 0.8f, 0.8f, 0.0f };
	light.direction = { 2.0f, 0.0f, 1.0f };

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, &light.color[0]);
	glUniform3fv(glGetUniformLocation(program, "lightDir"), 1, &light.direction[0]);
	glUniform1f(glGetUniformLocation(program, "ambientIntensity"), ambientIntensity);

	return true;
}

update_status ModuleRenderExercise::Update()
{
	ModuleModel* model = App->GetModel();
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	glUseProgram(program);

	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "proj");

	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection[0][0]);

	if (model->textures.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->textures[0]);
		glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	}

	glUniform3fv(glGetUniformLocation(program, "kd"), 1, &App->GetModel()->kd[0]);
	glUniform3fv(glGetUniformLocation(program, "ks"), 1, &App->GetModel()->ks[0]);
	glUniform1f(glGetUniformLocation(program, "n"), App->GetModel()->n);
	glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, &camera->position[0]);

	
	for (size_t i = 0; i < model->meshes.size(); ++i) {
		auto& mesh = model->meshes[i];
		glBindVertexArray(mesh->vao);
		//LOG("Mesh %s - VAO: %d, VBO: %d, EBO: %d, Vertices: %d, Indices: %d", mesh->name.c_str(), mesh->vao, mesh->vbo, mesh->ebo, mesh->vertexCount, mesh->indices.size());
		glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &mesh->modelMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
	}
	
	App->GetDraw()->Draw(view, projection, width, height);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp() {
	glDeleteProgram(program);
	return true;
}

void ModuleRenderExercise::UpdateCamera() {
	view = camera->GetViewMatrix();
}

void ModuleRenderExercise::FileDrop(const char* filePath) {
	std::string path = filePath;
	std::string extension = path.substr(path.find_last_of('.') + 1);
	bool modelLoaded = false;

	if (extension == "gltf" || extension == "glb") modelLoaded = App->GetModel()->Load(path.c_str());
	else App->GetModel()->LoadTexture(path.c_str());

	if (modelLoaded) {
		adjustCameraToGeometry();
	}
}

void ModuleRenderExercise::adjustCameraToGeometry() {
	float diagonal = Length(App->GetModel()->box.max - App->GetModel()->box.min);
	camera->SetPosition(App->GetModel()->center + float3(0.0f, 1.0f, -diagonal * 2.0f));

	camera->LookAt(App->GetModel()->center);
}