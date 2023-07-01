#include <pspctrl.h>
#include <stdio.h>

void loadstdconfig(void);
void refreshconfig(int entry,int key);
int loadsettings(void);
void savesettings(void);

#define std_menu     (PSP_CTRL_VOLDOWN | PSP_CTRL_VOLUP)
#define std_tvout    (PSP_CTRL_VOLDOWN | PSP_CTRL_RTRIGGER)
#define std_spb      (PSP_CTRL_VOLDOWN | PSP_CTRL_LTRIGGER)
#define std_switcher (PSP_CTRL_VOLDOWN | PSP_CTRL_SQUARE)

#define std_ofs0     0x04300000 - 0x150
#define std_ofs1     0x04300000 - 0x150 + (13*768*2)
#define std_ofs2     0x04300000 - 0xC0 + (19*2) + (13*768*2)
#define std_ofs3     0x04300000 - 0x150 + (50*768*2)


#define CONFIGPATH "ms0:/seplugins/fusaconfig.db"

typedef struct
{
	int menu;
	int tvout;
	int spb;
	int switcher;
	int offset0;
	int offset1;
	int offset2;
} FusaConfig;
