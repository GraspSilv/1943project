/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Timer.cpp
	Implementation of Timer class (tracks time for framerate)
History
	04/11/14	Bill Gowans		Create, define default constructor, start(), stop(), pause(), unpause(), get_ticks(), is_started(), and is_paused()
	04/28/14	Jon Richelsen	Standardize
*/
#include "Timer.h"
#include "SDL/SDL.h"

Timer::Timer() {
	paused = 0;
	started = 0;
	pausedTicks = 0;
	startTicks = 0;
}

void Timer::start() {
	paused = 0;
	started = 1;
	startTicks = SDL_GetTicks();
}

void Timer::stop() {
	paused = 0;
	started = 0;
}

void Timer::pause() {
	if(started && !paused) {
		paused = 1;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause() {
	if(paused) {
		paused = 0;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

int Timer::get_ticks() {
	if(started) {
		if(paused) {
			return pausedTicks;
		} else {
			return SDL_GetTicks() - startTicks;
		}
	}
  return 0;
}

int Timer::is_started() {
  return started;
}

int Timer::is_paused() {
  return paused;
}
