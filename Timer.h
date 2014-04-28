/*
Bill Gowans, Jack Magiera, Jon Richelsen
CSE20212
1943_Project
Timer.h
	Interface of Timer class (tracks time for framerate)
History
	04/11/14	Bill Gowans		Create, declare default constructor, start(), stop(), pause(), unpause(), get_ticks(), is_started(), is_paused(), and private variables startTicks, pausedTicks, paused, and started
	04/28/14	Jon Richelsen	Standardize
To Do
*/
#ifndef TIMER_H
#define TIMER_H

class Timer {
	public:
		Timer();
		void start();
		void stop();
		void pause();
		void unpause();
		int get_ticks();
		int is_started();
		int is_paused();
	private:
		int paused;
		int started;
		int pausedTicks;
		int startTicks;
};

#endif //TIMER_H
