#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Application.h"
#include <GL/glew.h>


ModuleCamera::ModuleCamera() {
	aspect = 0;
	width = 0;
	height = 0;
}

ModuleCamera::~ModuleCamera() {

}

bool ModuleCamera::Init() {
	return true;
}


void ModuleCamera::PerspectiveCamera(const float3& position, const float& verticalFov, const float& nearPlane, const float& farPlane) {
	frustum.type = FrustumType::PerspectiveFrustum;
	SetPosition(position);
	SetAspectRatio();
	SetFOV(verticalFov);
	SetPlaneDistances(nearPlane, farPlane);

	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
}

void ModuleCamera::ResizeCamera() {
	SetAspectRatio();
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
}

void ModuleCamera::SetFOV(const float& verticalFov = pi / 4.0){
	frustum.verticalFov = verticalFov;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
}

void ModuleCamera::SetAspectRatio(){
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	aspect = static_cast<float>(width) / static_cast<float>(height);
}

void ModuleCamera::SetPlaneDistances(const float& nearPlane = 0.1f, const float& farPlane = 200.0f){
	frustum.nearPlaneDistance = nearPlane;
	frustum.farPlaneDistance = farPlane;
}

void ModuleCamera::SetPosition(const float3& position = float3(0, 0, 0)){
	frustum.pos = position;
}

float3x3 CreatePitchMatrix(const float& pitch) {
	return float3x3{
		1.0f, 0.0f, 0.0f,
		0.0f, cos(pitch), -sin(pitch),
		0.0f, sin(pitch), cos(pitch)
	};
}

float3x3 CreateYawMatrix(const float& yaw) {
	return float3x3{
		cos(yaw), 0.0f, sin(yaw),
		0.0f, 1.0f, 0.0f,
		-sin(yaw), 0.0f, cos(yaw)
	};
}

void ModuleCamera::SetOrientation(const float& pitch, const float& yaw) {
	float3x3 pitchMatrix = CreatePitchMatrix(pitch);
	float3x3 yawMatrix = CreateYawMatrix(yaw);
	float3x3 rotationMatrix = yawMatrix * pitchMatrix;

	float3 oldFront = frustum.front.Normalized();
	frustum.front = rotationMatrix.MulDir(oldFront);

	float3 oldUp = frustum.up.Normalized();
	frustum.up = rotationMatrix.MulDir(oldUp);
}

void ModuleCamera::LookAt(const float3& target) {
	float3 front = (frustum.pos - target).Normalized();
	float3 right = front.Cross(frustum.up).Normalized();
	float3 up = right.Cross(front).Normalized();

    //frustum.front = front;
    //frustum.up = up;

	//Its not working good
	float3x3 rotationMatrix(right, up, front);

	frustum.front = rotationMatrix.WorldX();
	frustum.up = rotationMatrix.WorldY();
}

void ModuleCamera::SetProjectionMatrix(){
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); //<-- Important to transpose!

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
}

void ModuleCamera::SetViewMatrix(){
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);
}

float4x4 ModuleCamera::GetProjectionMatrix() {
	return frustum.ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix() {
	return float4x4(frustum.ViewMatrix());
}