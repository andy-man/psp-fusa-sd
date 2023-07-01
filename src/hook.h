#include <systemctrl.h>
#include <psputilsforkernel.h>
#include <pspdisplay_kernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>

int CtrlReadBufferN(SceCtrlData *pad_data, int count);
int CtrlReadBufferP(SceCtrlData *pad_data, int count);

int waitvblank(void);
int waitvblank2(void);
int getvcount(void);
int gethcount(void);
int SetMode(int, int, int);
int sceGeEdramGetSizeF(void);
void DisplaySetFrameBufferInternalF(int, void*, int, int, int);
void sceDisplaySetFrameBufF(void*, int, int, int);

void firstprepar(void);
void enableHooks(void);
void disableHooks(void);

unsigned int SetK1(unsigned int);
void stuba(void);
int SetModeF(int, int, int);
int setFrmBufInternal(int, void*, int, int, int);

int (*DisplaySetFrameBufferInternal) (int, void*, int, int, int);
int (*GeEdramGetSize) (void);
int (*DisplaySetFrameBuf)(void*, int, int, int);
int (*DisplaySetMode)(int, int, int);
void (*sceGeEdramSetSize)(int);

typedef struct
{
	int vcount;
	int AccHcount;
	int C;
	int LineTotal;
	int VblankEnd;
	int evid;
} DisplayVblank;
