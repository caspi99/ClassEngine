#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

class Application;
class ModuleWindow;
class ModuleModel;

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init();

	void CreateProjectionMatrix();
	void CreateViewMatrix();

	void PerspectiveCamera(const float3& position, const float& verticalFov, const float& nearPlane, const float& farPlane);
	void ResizeCamera();

	void SetFOV(const float& verticalFov);
	void SetAspectRatio();
	void SetPlaneDistances(const float& nearPlane, const float& farPlane);
	void SetPosition(const float3& position);
	void SetOrientation(const float& pitch, const float& yaw);
	void Orbit(const float& pitch, const float& yaw);
	void LookAt(const float3& target);

	float3 WorldRight();

	void SetProjectionMatrix();
	void SetViewMatrix();
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

	float3 position;
	float3 front;
	float3 up;

private:
	int width;
	int height;
	float aspect;
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float3 right;
	float nearPlane;
	float farPlane;
	float horizontalFOV;
	float verticalFOV;
};