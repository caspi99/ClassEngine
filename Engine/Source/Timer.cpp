#include "Timer.h"

MsTimer::MsTimer() {

}

MsTimer::~MsTimer() {

}

void MsTimer::Start() {
	if (!isRunning) {
		last_tick_time = SDL_GetTicks();
		isRunning = true;
	}
}

void MsTimer::Stop() {
	isRunning = false;
}

void MsTimer::Reset() {
	last_tick_time = SDL_GetTicks();
	deltaTime = 0;
	isRunning = true;
}

void MsTimer::Tick(){
	if (isRunning) {
		float tick_time = SDL_GetTicks();
		deltaTime = tick_time - last_tick_time;
		last_tick_time = tick_time;
	}
}

MsTimer::operator float() const {
	return deltaTime;
}

double UsTimer::frequency = (double)SDL_GetPerformanceFrequency();

UsTimer::UsTimer() {

}

UsTimer::~UsTimer() {

}

void UsTimer::Start() {
	if (!isRunning) {
		last_tick_time = SDL_GetPerformanceCounter();
		isRunning = true;
	}
}

void UsTimer::Stop() {
	isRunning = false;
}

void UsTimer::Reset() {
	last_tick_time = SDL_GetPerformanceCounter();
	time = 0;
	isRunning = true;
}

void UsTimer::Tick() {
	if (isRunning) {
		float tick_time = SDL_GetPerformanceCounter();
		time = (tick_time - last_tick_time) / frequency;
		last_tick_time = tick_time;
	}
}

UsTimer::operator double() const {
	return time;
}
