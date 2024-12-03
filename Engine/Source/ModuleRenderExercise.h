#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

class Application;
class ModuleCamera;
class ModuleProgram;
class ModuleWindow;
class ModuleDebugDraw;
class ModuleTexture;
class ModuleModel;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status Update();
	bool CleanUp();

	void UpdateCamera();

private:
	ModuleCamera* camera;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int program;

	float4x4 model;
	float4x4 view;
	float4x4 projection;

	int width;
	int height;
};