#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

class Application;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();

	void PerspectiveCamera(const float3& position, const float& verticalFov, const float& nearPlane, const float& farPlane);
	void ResizeCamera();

	void SetFOV(const float& verticalFov);
	void SetAspectRatio();
	void SetPlaneDistances(const float& nearPlane, const float& farPlane);
	void SetPosition(const float3& position);
	void SetOrientation(const float& pitch, const float& yaw);
	void LookAt(const float3& target);

	void SetProjectionMatrix();
	void SetViewMatrix();
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

	Frustum frustum;
private:
	int width;
	int height;
	float aspect;
};