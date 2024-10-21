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
	unsigned CompileShader(unsigned type, const char* shader_file_name);
	unsigned CreateProgram(const char* vertex_shader_file_name, const char* fragment_shader_file_name);
};