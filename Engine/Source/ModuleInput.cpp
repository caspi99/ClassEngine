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
            case SDL_MOUSEWHEEL:
                if (sdlEvent.wheel.y != 0) {
                    float zoomAmount = sdlEvent.wheel.y * 10.0f;
                    App->GetCamera()->position += App->GetCamera()->front * zoomAmount * deltaTime;
                    App->GetCamera()->CreateViewMatrix();
                }
                break;
            case SDL_DROPFILE:
                char* droppedFilePath = sdlEvent.drop.file;
                App->GetRenderExercise()->FileDrop(droppedFilePath);
                SDL_free(droppedFilePath);
                break;

        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT]) {
        speed = 0.3f;
    }
    else {
        speed = 0.15f;
    }

    //if (keyboard[SDL_SCANCODE_Q]) App->GetCamera()->position.y += speed * delta_time;
    //if (keyboard[SDL_SCANCODE_E]) App->GetCamera()->position.y -= speed * delta_time;
    
    if (keyboard[SDL_SCANCODE_F]) App->GetRenderExercise()->adjustCameraToGeometry();

    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && keyboard[SDL_SCANCODE_LALT]) {
        if (firstTime) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstTime = false;
        }

        int deltaX = mouseX - lastMouseX;
        int deltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        float yaw = deltaX * sensitivity * deltaTime;
        float pitch = deltaY * sensitivity * deltaTime;

        App->GetCamera()->Orbit(pitch, yaw);
    }
    else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        float3 movement = float3::zero;
        if (keyboard[SDL_SCANCODE_W]) movement += App->GetCamera()->front;
        if (keyboard[SDL_SCANCODE_S]) movement -= App->GetCamera()->front;
        if (keyboard[SDL_SCANCODE_A]) movement -= App->GetCamera()->WorldRight();
        if (keyboard[SDL_SCANCODE_D]) movement += App->GetCamera()->WorldRight();

        movement.Normalized();
        App->GetCamera()->position += movement * speed * deltaTime;

        if (firstTime) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstTime = false;
        }

        int deltaX = mouseX - lastMouseX;
        int deltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        float yaw = deltaX * sensitivity * deltaTime;
        float pitch = deltaY * sensitivity * deltaTime;

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
