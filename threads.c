// Copyright 2009-2013 Kevin Cox

/*******************************************************************************
*                                                                              *
*  This software is provided 'as-is', without any express or implied           *
*  warranty. In no event will the authors be held liable for any damages       *
*  arising from the use of this software.                                      *
*                                                                              *
*  Permission is granted to anyone to use this software for any purpose,       *
*  including commercial applications, and to alter it and redistribute it      *
*  freely, subject to the following restrictions:                              *
*                                                                              *
*  1. The origin of this software must not be misrepresented; you must not     *
*     claim that you wrote the original software. If you use this software in  *
*     a product, an acknowledgment in the product documentation would be       *
*     appreciated but is not required.                                         *
*                                                                              *
*  2. Altered source versions must be plainly marked as such, and must not be  *
*     misrepresented as being the original software.                           *
*                                                                              *
*  3. This notice may not be removed or altered from any source distribution.  *
*                                                                              *
*******************************************************************************/

/// Monitor a port continously and asyncronously.

#ifndef THREADS_C___PORT_MONITOR_LIBRARY
#define THREADS_C___PORT_MONITOR_LIBRARY

#include <stdlib.h>
#include <pthread.h>
#include <sys/io.h>
#include <unistd.h>

#include "threads.h"

pthread_t monitor_thread;
unsigned int monitor_port;

/** \internal
	\brief Calls monitor_check_changes() continusly

	An internal function that is used as the thread function for
	monitor_start_thread().  It calls monitor_update_output() over and over again.
 */
int monitor_loop ( void *d )
{
	for (;;)
	{
		monitor_check_changes(d, inb(monitor_port)); // NOTE: d is a pointer
		usleep(0); // Give a cancellation point
	}
}

/// Starts the background thread to check for changes
/**
 \note It is not nessary to call monitor_int() when calling this function.
 \warning All settings will be lost when calling this function
	(Because it calls monitor_init())

 \param port port to monitor for changes.

 \return Returns 0 on success and another value on error.
 */
int monitor_start_thread ( monitor_data *d, unsigned int port )
{
	monitor_init ( d, port ); // NOTE: d is a pointer

	return pthread_create(&monitor_thread, NULL, (void*)monitor_loop, d); // NOTE: d is a pointer
}

/// Kills the background thread
/** Kills the background monitor thread created by monitor_pwm_thread()

	\warning You must call monitor_start_thread() before this function and since
		the	last call of this function.
 */
int monitor_kill_thread ( void )
{
	pthread_cancel(monitor_thread);

	int ret;	//	Return Value of the thread
	ret = pthread_join(monitor_thread, NULL);

	return ret;
}

#endif
