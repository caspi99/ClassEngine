#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "ModuleModel.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderExercise = new ModuleRenderExercise());
	modules.push_back(draw = new ModuleDebugDraw());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(model = new ModuleModel());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(input = new ModuleInput());

	lastTime = 0;
	deltaTime = 0;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	double currentTime = SDL_GetPerformanceCounter();
	deltaTime = (double)((currentTime - lastTime) / (double)SDL_GetPerformanceFrequency());
	lastTime = currentTime;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	for (const char* message : App->logMessages) {
		free(const_cast<char*>(message));
	}
	App->logMessages.clear();

	return ret;
}
