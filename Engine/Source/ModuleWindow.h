#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include <set>
#include <vector>

class Application;
class ModuleCamera;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	update_status Update();

	// Called before quitting
	bool CleanUp();

	void ChangeWindowMode(const int windowMode);
	void RenderResolutionSelector();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	std::set<std::pair<int, int>> resolutions;
	std::vector<const char*> resolutionStrings;
	int selectedResolution = 0;
};

#endif // __ModuleWindow_H__