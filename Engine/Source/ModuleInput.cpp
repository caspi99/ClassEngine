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
        speed = 0.02f;
    }
    else {
        speed = 0.01f;
    }

    // Camera movement
    if (keyboard[SDL_SCANCODE_Q]) App->GetCamera()->frustum.pos.y += speed;
    if (keyboard[SDL_SCANCODE_E]) App->GetCamera()->frustum.pos.y -= speed;
    if (keyboard[SDL_SCANCODE_W]) App->GetCamera()->frustum.pos += App->GetCamera()->frustum.front * speed;
    if (keyboard[SDL_SCANCODE_S]) App->GetCamera()->frustum.pos -= App->GetCamera()->frustum.front * speed;
    if (keyboard[SDL_SCANCODE_A]) App->GetCamera()->frustum.pos -= App->GetCamera()->frustum.WorldRight() * speed;
    if (keyboard[SDL_SCANCODE_D]) App->GetCamera()->frustum.pos += App->GetCamera()->frustum.WorldRight() * speed;

    /*
    * 
    * 
    * 
    Esto es para comparar
    auto Game::handleMouseState(float fps) -> void {
  _mouseInput->mouseState =
      SDL_GetMouseState(&_mouseInput->mouseX, &_mouseInput->mouseY);
  if (_mouseInput->mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    // printf("SDL_MOUSEBUTTONDOWN\n");
    auto halfWindowWidth = _graphics->getWindowWidth() / 2;
    auto halfWindowHeight = _graphics->getWindowHeight() / 2;
    _gameState->setCamera(
        {_gameState->getPlayer()->getPosition().x - halfWindowWidth,
         _gameState->getPlayer()->getPosition().y - halfWindowHeight});
    _gameState->getPlayer()->onDestinationSelected(
        {(float)_mouseInput->mouseX + _gameState->getCamera().x,
         (float)_mouseInput->mouseY + _gameState->getCamera().y},
        fps);

    validatePlayerPosition();
  }




    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseState && SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
        int centerX = width / 2;
        int centerY = height / 2;

        SDL_WarpMouseInWindow(App->GetWindow()->window, centerX, centerY);

        int deltaX = mouseX - centerX;
        int deltaY = mouseY - centerY;

        float sensitivity = 0.001f;
        float yaw = deltaX * sensitivity;
        float pitch = deltaY * sensitivity;

        App->GetCamera()->SetOrientation(pitch, yaw);
    }*/

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
