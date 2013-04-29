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

/** \mainpage Monitor Library
 *
 * \section intro_sec Introduction
 *
 * The Monitor library was created to simplify the monitoring of the parallel port.
 *
 *
 * \subsection files Files
 *      Main Header: monitor/monitor.h
 *		Threading support: monitor/threads.h
 **/

#ifndef MONITOR_H___PORT_MONITOR_LIBRARY
#define MONITOR_H___PORT_MONITOR_LIBRARY

/** \file monitor.h
    \brief Main header file
*/

#include <stdint.h>

/// Styles of monitoring
typedef enum _monitor_check_style
{
	low,  ///< When the port drops from high to low
	high, ///< When the port goes from low to high
	both, ///< Whenever the port changes
	none  ///< Never
} monitor_check_style;

typedef struct _monitor_data monitor_data;

/// Callback function for changes
/**
 * This is the template for callback functions.
 * 
 * The state of \a monitor_data will be with the new byte.  The order in which
 * the callbacks are called is undefined.
 *
 * \param 1 A pointer to the monitor object that called the callback.
 * \param 2 The first argument is the bit that has changed.
 * \param 3 The second argument is the new state of the bit.
 * \param 4 The data passed to monitor_set_callback() for this bit.
 */
typedef void (*monitor_calback)(monitor_data*, unsigned short, unsigned short, void*);

struct _monitor_data
{
	uint8_t state;

	monitor_calback cb_high[8];
	void *cb_high_data[8];
	monitor_calback cb_low[8];
	void *cb_low_data[8];
};

/// Initilize the monitor structure.
/**
 * This must be called before any other functions.  If called afterwords it
 * will overwrite your settings.  If not called the results are undefined.
 * 
 * \note monitor_start_thread() calls this for you.
 * 
 * \param d a monitor_data structure that has been initilized by monitor_init().
 * \param port The current state of the thing to monitor.  We need this so that
 *   we know what changed on the first call.
 */
void monitor_init ( monitor_data *d, uint8_t curstate );

/// Sets a callback function for when changes occour
/**
 * Sets a function to be called when the change of the style \a style occurs.
 * \c high and \c low will only affect that action while \c both and \c none
 * will set both.  This means that you can have a different high and low
 * callbacks by calling this function twice.
 * 
 * \param d a monitor_data structure that has been initilized by monitor_init().
 * \param bit what bit to monitor
 * \param style The desired style to watch for.
 * \param callback \a Callback will be called when \a bit experiences a change
 *   that satisfies \a style.  \c NULL can be passed to disable that trigger.
 * \param data A pointer to be passed to the callback.
 */
void monitor_set_callback(monitor_data *d,
                          unsigned short bit, monitor_check_style style,
                          monitor_calback callback, void *data
                         );
                         
/// Check for changes
/** 
 * Checks for changes and calls the appropriate callback functions with the
 * proper arguments.
 * 
 * \param d a monitor_data structure that has been initialized by monitor_init().
 * \param state The state of the device to monitor.
 * \return Returns the number of bits that have changed (not callbacks or
 *   watched bits but total number that have changed).
 */
unsigned short monitor_check_changes ( monitor_data *d, uint8_t state );

/// Get the state.
/**
 * Returns the value from the last call to monitor_check_changes(). This is
 * useful as you don't have to save the value passed in beforehand and you can
 * call it where ever you have a reference to the monitor_data object.
 * 
 * \param d a monitor_data structure that has been initialized by monitor_init().
 */
uint8_t monitor_get_state ( monitor_data *d );

/// Get the state of a bit.
/**
 * Returns the value of bit \a bit from the last call to monitor_check_changes().
 * This is useful as you don't have to save the value passed in beforehand and
 * you can call it where ever you have a reference to the monitor_data object.
 * 
 * \param d a monitor_data structure that has been initialized by monitor_init().
 * \param bit The bit to get.
 */
unsigned short monitor_get_bit ( monitor_data *d, unsigned short bit );

#endif
