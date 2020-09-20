#include "companion.h"
#include "updates.h"

void initUpdates()
{
	btEnabled();
}

// Check if Bluetooth is enabled
int btEnabled()
{
	// Run command and place output in buffer
	FILE *p = popen("bluetoothctl show | grep Powered", "r");
	char buffer[100];
	fgets(buffer,sizeof(buffer),p);
	pclose(p);
	
	if(strstr(buffer,"yes") != NULL)
	{
		printf("Bluetooth is enabled\n");
		return 1;
	}
	else
	{
		printf("Bluetooth is not enabled\n");
		return 0;
	}
}

