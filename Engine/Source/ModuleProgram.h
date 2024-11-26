#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

public:
	char* LoadShaderSource(const char* shader_file_name);
	unsigned int CompileShader(unsigned int type, const char* shader_file_name);
	unsigned int CreateProgram(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
};