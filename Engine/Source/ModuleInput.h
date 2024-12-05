#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleOpenGL;
class ModuleCamera;
class ModuleWindow;
class Application;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	const Uint8 *keyboard = NULL;
	int width;
	int height;
	float speed = 0.1f;
	float sensitivity = 1.5f;
	int lastMouseX = 0;
	int lastMouseY = 0;
	bool firstTime = true;
};