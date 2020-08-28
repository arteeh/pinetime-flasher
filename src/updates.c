#include "pinetime-companion.h"
#include "updates.h"

void initUpdates()
{
	g_print("Bluetooth enabled: %i\n",btEnabled());
}

// Check if Bluetooth is enabled
int btEnabled()
{
	FILE *p = popen("bluetoothctl show | grep Powered", "r");
	char buffer[100];
	fgets(buffer,sizeof(buffer),p);
	pclose(p);
	
	if(strstr(buffer,"yes") != NULL) return 1;
	else return 0;
}

