#include "ModuleRenderExercise.h"
#include <GL/glew.h>
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"

ModuleRenderExercise::ModuleRenderExercise() {
	vao = 0;
	vbo = 0;
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
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f 
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Create program with shaders
	program = App->GetProgram()->CreateProgram("default_vertex.glsl", "default_fragment.glsl");
	if (program == 0) {
		LOG("Error: Failed to create shader program");
		return false;
	}

	model = float4x4::FromTRS(float3(1.0f, 0.0f, -10.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 1.0f));

	//Camera working
	camera = App->GetCamera();
	camera->PerspectiveCamera(float3(0.0f, 1.0f, -2.0f), pi / 4.0f, 0.1f, 200.0f);
	//camera->LookAt(float3(0.0f, 0.0f, 10.0f)); //Creo que esta al reves, preguntar
	UpdateCamera();

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

	glBindVertexArray(vao);
	// 1 triangle to draw = 3 vertices 
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	App->GetDraw()->Draw(view, projection, width, height);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);
	return true;
}

void ModuleRenderExercise::UpdateCamera() {
	projection = camera->GetProjectionMatrix();
	view = camera->GetViewMatrix();
}