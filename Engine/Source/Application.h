#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include <vector>
#include "Timer.h"

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

extern MsTimer deltaTime;
extern UsTimer realTime;

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

    std::vector<const char*> logMessages;

    void print(const char* message) {
        logMessages.push_back(message);
    }


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
};

extern Application* App;
