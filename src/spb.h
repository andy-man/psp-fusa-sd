#include "hook.h"
#include "systimer.h"

int (*pspVblankInterruptHandler)(int);
void speedboost_enable(void);
void speedboost_disable(void);

int addr_SetFrameBuf;
int addr_GetFrameBuf;
int addr_SetFrameBufInt;
int addr_SetMode;
