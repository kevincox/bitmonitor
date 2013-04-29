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

/** \file threads.h
 *  \brief Monitor header file
 *
 *  This file will allow users to use threading so that they don't have to worry
 *  about calling monitor_check_changes() manually.  It only works for
 *  monitoring GPIO and parallel ports.
 *
 * \note This file must be compiled and linked with -pthread.
 */

#ifndef THREADS_H___PORT_MONITOR_LIBRARY
#define THREADS_H___PORT_MONITOR_LIBRARY

#include "monitor.h"

int monitor_loop ( void *d );
int monitor_start_thread ( monitor_data *d, unsigned int port );
int monitor_kill_thread ( void );

#endif
