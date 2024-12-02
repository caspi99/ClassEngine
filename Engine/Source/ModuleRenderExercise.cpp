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
	vao = 0;
	vbo = 0;
	ebo = 0;
	program = 0;
	width = 0;
	height = 0;
	camera = nullptr;
}

ModuleRenderExercise::~ModuleRenderExercise() {

}

bool ModuleRenderExercise::Init() {
	//Triangle VBO
	float vtx_data[] = {
		-1.0f, -1.0f, 0.0f,      0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,      1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,      0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,      1.0f, 0.0f
	};

	unsigned int vtx_indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vtx_indices), vtx_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));

	//Create program with shaders
	program = App->GetProgram()->CreateProgram("default_vertex.glsl", "default_fragment.glsl");
	if (program == 0) {
		LOG("Error: Failed to create shader program");
		return false;
	}

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -10.0f),
		float4x4::RotateZ(0.0f),
		float3(1.0f, 1.0f, 1.0f));

	//Camera working
	camera = App->GetCamera();
	camera->PerspectiveCamera(float3(0.0f, 1.0f, -2.0f), pi / 4.0f, 0.1f, 200.0f);
	//camera->LookAt(float3(0.0f, 0.0f, 10.0f)); //Creo que esta al reves, preguntar
	UpdateCamera();

	bool isTexture = App->GetTexture()->getTexture(L"Baboon.ppm");
	App->GetModel()->Load("Box.gltf");

	return true;
}

update_status ModuleRenderExercise::Update()
{
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	glUseProgram(program);

	GLint modelLoc = glGetUniformLocation(program, "model");
	GLint viewLoc = glGetUniformLocation(program, "view");
	GLint projLoc = glGetUniformLocation(program, "proj");

	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(viewLoc, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_TRUE, &projection[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->GetTexture()->texture);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);

	glBindVertexArray(vao);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	App->GetDraw()->Draw(view, projection, width, height);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);
	return true;
}

void ModuleRenderExercise::UpdateCamera() {
	projection = camera->GetProjectionMatrix();
	view = camera->GetViewMatrix();
}