#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		ChangeWindowMode(1);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

void ModuleWindow::ChangeWindowMode(const int windowMode) {
	int width = 0, height = 0;
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
		width = displayMode.w;
		height = displayMode.h;
	}

	if (windowMode == 0) {
		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
			LOG("Error setting fullscreen mode: %s", SDL_GetError());
		}
	}
	else if (windowMode == 1) {
		SDL_Rect usableBounds;
		if (SDL_SetWindowFullscreen(window, 0) != 0) {
			LOG("Error exiting fullscreen mode: %s", SDL_GetError());
		}
		if (SDL_GetDisplayUsableBounds(0, &usableBounds) != 0) {
			LOG("Error getting usable bounds: %s", SDL_GetError());
		}
		SDL_SetWindowSize(window, usableBounds.w, usableBounds.h);
		SDL_SetWindowPosition(window, usableBounds.x, usableBounds.y);
		SDL_SetWindowBordered(window, SDL_TRUE);
	} 
	else if (windowMode == 2) {
		if (SDL_SetWindowFullscreen(window, 0) != 0) {
			LOG("Error setting windowed mode: %s", SDL_GetError());
		}
		SDL_SetWindowSize(window, width, height);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

update_status ModuleWindow::Update()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

