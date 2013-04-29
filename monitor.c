#ifndef MONITOR_C___PORT_MONITOR_LIBRARY
#define MONITOR_C___PORT_MONITOR_LIBRARY

#include <stdlib.h>
#include <sys/time.h>

#include "monitor.h"

void monitor_init ( monitor_data *d, uint8_t curstate )
{
	int i;

	d->state = curstate;

	for ( i = 0; i < 8; i++ )
	{
		d->cb_low[i]  = NULL;
		d->cb_high[i] = NULL;
	}
}

void monitor_set_callback ( monitor_data *d, 
                            unsigned short bit, monitor_check_style style,
                            monitor_calback callback, void *data )
{
	if ( style == low || style == both )
	{
		d->cb_low[bit] = callback;
		d->cb_low_data[bit] = data;
	}
	if ( style == high || style == both )
	{
		d->cb_high[bit] = callback;
		d->cb_high_data[bit] = data;
	}
	if ( style == none )
	{
		d->cb_low[bit]       = NULL;
		d->cb_low_data[bit]  = NULL;
		d->cb_high[bit]      = NULL;
		d->cb_high_data[bit] = NULL;
	}
}

unsigned short monitor_check_changes ( monitor_data *d, uint8_t state )
{
	int i;
	short unsigned r = 0; // If there were any changes

	uint8_t c = d->state ^ state; // Changed bits.
	
	d->state = state; // Save the new state.

	//printf("[Monitor] Old state: 0x%x\tNew state: 0x%x\n", o, d->state);

	for ( i = 8; i--; )
	{
		uint8_t m = 1 << i;	 // Make a bit mask
		if ( m & c ) // If the bit changed
		{
			r++;
			if ( ( d->state & m ) && d->cb_high[i] )
				(d->cb_high[i])(d, i, 1, d->cb_high_data[i]); // Went high.
			else if ( d->cb_low[i] )
				(d->cb_low[i])(d, i, 0, d->cb_low_data[i]);   // Went low.
		}
	}
	return r;
}

uint8_t monitor_get_state ( monitor_data *d )
{
	return d->state;
}
unsigned short monitor_get_bit ( monitor_data *d, unsigned short bit )
{
	int m = 1 << bit; // Create a bit mask.

	return (d->state & m) != 0;
}

#endif
