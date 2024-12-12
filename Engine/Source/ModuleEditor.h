#pragma once
#include "Module.h"
#include "Globals.h"

class Application;
class ModuleWindow;
class ModuleOpenGL;
class ModuleTexture;
class ModuleModel;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	update_status GeneralMenu();
	void ConfigMenu();
	void UpdateFPS();
	void LogConsole();

private:
	bool editorWindowShow;
	float fps = 0.0f;
	const int FPS_HISTORY_SIZE = 100;
	float fpsHistory[100] = { 0 };
	int fpsHistoryIndex = 0;
	int width;
	int height;
	int maxWidth;
	int maxHeight;
	int windowMode;
};
