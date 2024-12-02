#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleRenderExercise.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui_impl_sdl2.h"

ModuleInput::ModuleInput() {
    width = 0;
    height = 0;
}

// Destructor
ModuleInput::~ModuleInput() {

}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT]) {
        speed = 10.f;
    }
    else {
        speed = 5.f;
    }

    float delta_time = App->deltaTime;

    // Camera movement
    if (keyboard[SDL_SCANCODE_Q]) App->GetCamera()->frustum.pos.y += speed * delta_time;
    if (keyboard[SDL_SCANCODE_E]) App->GetCamera()->frustum.pos.y -= speed * delta_time;

    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        if (keyboard[SDL_SCANCODE_W]) App->GetCamera()->frustum.pos += App->GetCamera()->frustum.front * speed * delta_time;
        if (keyboard[SDL_SCANCODE_S]) App->GetCamera()->frustum.pos -= App->GetCamera()->frustum.front * speed * delta_time;
        if (keyboard[SDL_SCANCODE_A]) App->GetCamera()->frustum.pos -= App->GetCamera()->frustum.WorldRight() * speed * delta_time;
        if (keyboard[SDL_SCANCODE_D]) App->GetCamera()->frustum.pos += App->GetCamera()->frustum.WorldRight() * speed * delta_time;

        if (firstTime) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstTime = false;
        }

        int deltaX = mouseX - lastMouseX;
        int deltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        float sensitivity = 2.0f;
        float yaw = deltaX * sensitivity * delta_time;
        float pitch = deltaY * sensitivity * delta_time;

        App->GetCamera()->SetOrientation(pitch, yaw);
    } 
    else {
        firstTime = true;
    }

    App->GetRenderExercise()->UpdateCamera();

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
