#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleModel.h"
#include <GL/glew.h>
#include <algorithm>


ModuleCamera::ModuleCamera() {
	aspect = 0;
	width = 0;
	height = 0;
	projectionMatrix = float4x4::zero;
	viewMatrix = float4x4::zero;
	front = float3::zero;
	up = float3::zero;
}

ModuleCamera::~ModuleCamera() {

}

bool ModuleCamera::Init() {
	return true;
}

void ModuleCamera::CreateProjectionMatrix() {
	float tanHalfFov = tan(verticalFOV/2);

	projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFov);
	projectionMatrix[1][1] = 1.0f / tanHalfFov;
	projectionMatrix[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projectionMatrix[2][3] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
	projectionMatrix[3][2] = -1.0f;
}

void ModuleCamera::CreateViewMatrix() {
	viewMatrix[0][0] = right.x;
	viewMatrix[0][1] = right.y;
	viewMatrix[0][2] = right.z;
	viewMatrix[0][3] = -Dot(position, right);

	viewMatrix[1][0] = up.x;
	viewMatrix[1][1] = up.y;
	viewMatrix[1][2] = up.z;
	viewMatrix[1][3] = -Dot(position, up);

	viewMatrix[2][0] = -front.x;
	viewMatrix[2][1] = -front.y;
	viewMatrix[2][2] = -front.z;
	viewMatrix[2][3] = Dot(position, front);

	viewMatrix[3][3] = 1.0f;
}

void ModuleCamera::PerspectiveCamera(const float3& position, const float& verticalFov, const float& nearPlane, const float& farPlane) {
	SetPosition(position);
	SetAspectRatio();
	SetFOV(verticalFov);
	SetPlaneDistances(nearPlane, farPlane);

	front = -float3::unitZ;
	up = float3::unitY;
	
	CreateProjectionMatrix();
	LookAt(float3(0, 0, 0));
}

void ModuleCamera::ResizeCamera() {
	SetAspectRatio();
	horizontalFOV = 2.f * atanf(tanf(verticalFOV * 0.5f) * aspect);
	CreateProjectionMatrix();
}

void ModuleCamera::SetFOV(const float& verticalFov = pi / 4.0){
	this->verticalFOV = verticalFov;
	this->horizontalFOV = 2.f * atanf(tanf(this->verticalFOV * 0.5f) * aspect);
}

void ModuleCamera::SetAspectRatio(){
	SDL_GetWindowSize(App->GetWindow()->window, &width, &height);
	aspect = static_cast<float>(width) / static_cast<float>(height);
}

void ModuleCamera::SetPlaneDistances(const float& nearPlane = 0.1f, const float& farPlane = 200.0f){
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

void ModuleCamera::SetPosition(const float3& position = float3(0, 0, 0)){
	this->position= position;
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
	Quat pitchQuat = Quat::RotateAxisAngle(right, pitch);
	Quat yawQuat = Quat::RotateAxisAngle(float3::unitY, yaw);
	Quat orientation = yawQuat * pitchQuat;
	orientation.Normalize();

	float3 oldFront = front.Normalized();
	front = orientation.Mul(oldFront).Normalized();

	float3 oldUp = up.Normalized();
	up = orientation.Mul(oldUp).Normalized();

	right = Cross(front, up).Normalized();

	CreateViewMatrix();
}

void ModuleCamera::Orbit(const float& pitch, const float& yaw) {
	float3 direction = position - App->GetModel()->center;

	Quat pitchQuat = Quat::RotateAxisAngle(right, pitch);
	Quat yawQuat = Quat::RotateAxisAngle(float3::unitY, yaw);
	Quat rotation = yawQuat * pitchQuat;
	rotation.Normalize();

	direction = rotation.Mul(direction);

	position = App->GetModel()->center + direction;

	LookAt(App->GetModel()->center);
}


void ModuleCamera::LookAt(const float3& target) {
	front = (target - position).Normalized();
	right = Cross(front, up).Normalized();
	up = Cross(right, front).Normalized();

	CreateViewMatrix();
}

float3 ModuleCamera::WorldRight() {
	return Cross(front, up);
}

void ModuleCamera::SetProjectionMatrix(){
	float4x4 projectionGL = projectionMatrix.Transposed();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
}

void ModuleCamera::SetViewMatrix(){
	float4x4 viewGL = viewMatrix.Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);
}

float4x4 ModuleCamera::GetProjectionMatrix() {
	return projectionMatrix;
}

float4x4 ModuleCamera::GetViewMatrix() {
	return viewMatrix;
}