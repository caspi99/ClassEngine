#pragma once
#include "Module.h"
#include "Globals.h"

class Application;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status Update();
	bool CleanUp();
};