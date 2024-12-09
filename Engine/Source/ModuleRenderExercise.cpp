#include "ModuleRenderExercise.h"
#include <GL/glew.h>
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"

ModuleRenderExercise::ModuleRenderExercise() {
	program = 0;
	width = 0;
	height = 0;
	camera = nullptr;
}

ModuleRenderExercise::~ModuleRenderExercise() {

}

bool ModuleRenderExercise::Init() {
	//Create program with shaders
	program = App->GetProgram()->CreateProgram("default_vertex.glsl", "default_fragment.glsl");
	if (program == 0) {
		LOG("Error: Failed to create shader program");
		return false;
	}

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -10.0f),
		float4x4::RotateZ(0.0f),
		float3(3.f, 3.f, 3.f));

	//Camera working
	camera = App->GetCamera();
	camera->PerspectiveCamera(float3(0.0f, 1.0f, -2.0f), pi / 4.0f, 0.1f, 10000.0f);
	camera->LookAt(float3(0.0f, -1.0f, 10.0f)); //Creo que esta al reves, preguntar
	UpdateCamera();
	
	//App->GetModel()->Load("Duck.gltf");
	App->GetModel()->Load("BakerHouse.gltf");

	projection = camera->GetProjectionMatrix();

	return true;
}

update_status ModuleRenderExercise::Update()
{
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	glUseProgram(program);

	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "proj");

	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection[0][0]);

	if (App->GetModel()->textures.size() > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, App->GetModel()->textures[0]);
		glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	}
	
	for (size_t i = 0; i < App->GetModel()->meshes.size(); ++i) {
		auto& mesh = App->GetModel()->meshes[i];
		glBindVertexArray(mesh->vao);
		LOG("Mesh %s - VAO: %d, VBO: %d, EBO: %d, Vertices: %d, Indices: %d", mesh->name.c_str(), mesh->vao, mesh->vbo, mesh->ebo, mesh->vertexCount, mesh->indices.size());
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
	float3 min = { 1000000000, 10000000000, 10000000000 };
	float3 max = { -1000000000, -10000000000, -10000000000 };
		
	for (size_t i = 0; i < App->GetModel()->meshes.size(); ++i) {
		auto& mesh = App->GetModel()->meshes[i];
		min = Min(min, mesh->box.min);
		max = Max(max, mesh->box.max);
	}

	float3 center = (min + max) * 0.5f;

	float diagonal = Length(max - min);
	camera->SetPosition(center + float3(0.0f, -1.0f, diagonal * 2.0f));

	camera->LookAt(center);
}