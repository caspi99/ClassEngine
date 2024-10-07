#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleOpenGL::ModuleOpenGL()
{

}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //Depth Buffer 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); //Stencil Buffer 8 bits

	context = SDL_GL_CreateContext(App->GetWindow()->window);
	
	GLenum err = glewInit();

	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); //Counter clockwise

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	int width;
	int height;
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
}

