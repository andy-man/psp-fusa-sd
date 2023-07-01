#include "config.h"

extern FusaConfig config;

void loadstdconfig(void)
{
config.menu = std_menu;
config.tvout = std_tvout;
config.spb = std_spb;
config.switcher = std_switcher;
config.offset0 = std_ofs0;
config.offset1 = std_ofs1;
config.offset2 = std_ofs2;
}

void refreshconfig(int entry,int key)
{

key &= ~(PSP_CTRL_WLAN_UP|PSP_CTRL_REMOTE|PSP_CTRL_DISC|PSP_CTRL_MS);

switch (entry)
{
case 0:
	config.menu = key;
	break;
case 1:
	config.tvout = key;
	break;
case 2:
	config.spb = key;
	break;
case 3:
	config.switcher = key;
}
}

int loadsettings()
{
	int fd;
	fd = sceIoOpen(CONFIGPATH, PSP_O_RDONLY, 0777);
	
	if(fd >= 0)
	{
		sceIoRead(fd, &config.menu, sizeof(config.menu));
		sceIoRead(fd, &config.tvout, sizeof(config.tvout));
		sceIoRead(fd, &config.spb, sizeof(config.spb));
		sceIoRead(fd, &config.switcher, sizeof(config.switcher));
		sceIoRead(fd, &config.offset0, sizeof(config.offset0));
		sceIoRead(fd, &config.offset1, sizeof(config.offset1));
		sceIoRead(fd, &config.offset2, sizeof(config.offset2));
		sceIoClose(fd);
	};
	
	return fd;
}

void savesettings(void)
{
	int fd;
	fd = sceIoOpen(CONFIGPATH, PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	
	if(fd >= 0)
	{
		sceIoWrite(fd, &config.menu, sizeof(config.menu));
		sceIoWrite(fd, &config.tvout, sizeof(config.tvout));
		sceIoWrite(fd, &config.spb, sizeof(config.spb));
		sceIoWrite(fd, &config.switcher, sizeof(config.switcher));
		sceIoWrite(fd, &config.offset0, sizeof(config.offset0));
		sceIoWrite(fd, &config.offset1, sizeof(config.offset1));
		sceIoWrite(fd, &config.offset2, sizeof(config.offset2));
		sceIoClose(fd);
		
	};
}
