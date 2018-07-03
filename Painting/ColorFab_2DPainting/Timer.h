#pragma once
//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
// AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
// MODIFIED: P. PUNPONGSANON (mojiiz.p@gmail.com)
// UPDATED: 2013-03-17
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#ifdef WIN32	// Windows system specific
// Solve the Qt bug: http://qt-project.org/forums/viewthread/22133
#define NOMINMAX
#include <QtCore/QThread>
#include <windows.h>
#elif WIN64		// Windows system specific
#define NOMINMAX
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

class Timer : public QThread
{
	public:
		Timer();														// default constructor
		~Timer();														// default destructor

		void   start();													// start timer
		void   stop();													// stop the timer
		double getElapsedTime();										// get elapsed time in second
		double getElapsedTimeInSec();									// get elapsed time in second (same as getElapsedTime)
		double getElapsedTimeInMilliSec();								// get elapsed time in milli-second
		double getElapsedTimeInMicroSec();								// get elapsed time in micro-second
		void writeElapsedTimeInMilliSec(char mode);						// get elapsed time in milli-second (enhanced for experimental)


	protected:


	private:
		double startTimeInMicroSec;											// starting time in micro-second
		double endTimeInMicroSec;											// ending time in micro-second
		int    stopped;														// stop flag 
	#ifdef WIN32
		LARGE_INTEGER frequency;											// ticks per second
		LARGE_INTEGER startCount;											//
		LARGE_INTEGER endCount;												//
	#else
		timeval startCount;													//
		timeval endCount;													//
	#endif
};

#endif // TIMER_H_DEF
