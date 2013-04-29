#include <stdio.h>

#include "monitor.h"

void callback(monitor_data *d, unsigned short bit, unsigned short state, void *data)
{
	printf("Bit %d changed to %d.\n", bit, state);
}

int main (void)
{
	monitor_data d;
	monitor_init(&d, 0);
	
	monitor_set_callback(&d, 0, both, &callback, NULL);
	monitor_set_callback(&d, 1, both, &callback, NULL);
	monitor_set_callback(&d, 2, both, &callback, NULL);
	monitor_set_callback(&d, 3, both, &callback, NULL);
	
	monitor_check_changes(&d, 4);
	// Bit 2 changed to 1.
	
	monitor_set_callback(&d, 1, both, NULL, NULL);
	
	monitor_check_changes(&d, 3);
	// Bit 0 changed to 1.
	// Bit 2 changed to 0.
	
	return 0;
}
