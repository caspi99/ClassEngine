#pragma once
#include <SDL.h>

class MsTimer
{
public:
	MsTimer();
	~MsTimer();

	void Start();
	void Stop();
	void Reset();
	void Tick();
	operator float() const;

private:
	float last_tick_time = 0;
	float deltaTime = 0;
	bool isRunning = false;
};

class UsTimer
{
public:
	UsTimer();
	~UsTimer();

	void Start();
	void Stop();
	void Reset();
	void Tick();
	operator double() const;

private:
	double last_tick_time = 0;
	double time = 0;
	bool isRunning = false;
	static double frequency;
};