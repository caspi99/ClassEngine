#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleCamera;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleEditor;
class ModuleTexture;
class ModuleModel;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleProgram* GetProgram() { return program; }
    ModuleRenderExercise* GetRenderExercise() { return renderExercise; }
    ModuleInput*  GetInput() { return input; }
    ModuleDebugDraw* GetDraw() { return draw; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleEditor* GetEditor() { return editor; }
    ModuleTexture* GetTexture() { return texture; }
    ModuleModel* GetModel() { return model; }

    double deltaTime;

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* renderExercise = nullptr;
    ModuleInput* input = nullptr;
    ModuleDebugDraw* draw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleModel* model = nullptr;

    std::list<Module*> modules;

    double lastTime;
};

extern Application* App;
