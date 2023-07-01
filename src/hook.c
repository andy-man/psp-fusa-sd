#include "hook.h"

extern DisplayVblank VBlank;
extern char lcd, enabled, refresh, ready;
extern SceUID scrsema, screvent;
extern void *address0, *address2;
extern int psm2, bufwidth2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// SERVICE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int CtrlReadBufferN(SceCtrlData *pad_data, int count)
{
//sceKernelDelayThread(1000);
return sceCtrlPeekBufferNegative(pad_data,count);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CtrlReadBufferP(SceCtrlData *pad_data, int count)
{
//sceKernelDelayThread(1000);
return sceCtrlPeekBufferPositive(pad_data,count);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int waitvblank (void)
{
	int k1 = SetK1(0);
	 int res = sceKernelWaitEventFlag(VBlank.evid, 1, 1, 0, 0);
	 if (res < 0) return(res);
	SetK1(k1);
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int waitvblank2 (void)
{
	return(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int getvcount (void)
{
	return (VBlank.vcount & 0x7FFFFFFF);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int SetMode (int mode, int width, int height)
{
	if (lcd) { lcd = 0; return SetModeF(mode, width, height); }
	else return SetModeF(0x1d1, 720, 503);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int sceGeEdramGetSizeF (void)
{
	return (0x200000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplaySetFrameBufferInternalF(int m, void* addr, int bw, int psm, int s)
{
	unsigned int k1;
	k1 = SetK1(0);
	if (m == 0) 
	{
	address0 = addr;
	addr = (void*)0x04300000;
	bw = 512;
	if (address0 == 0) psm = (psm2 == 3) ? 0 : psm2;
	};

	SetK1(k1);
	setFrmBufInternal( m, addr, bw, psm, s );  // call original setfrmbfr :P PSP_DISPLAY_SETBUF_IMMEDIATE
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void sceDisplaySetFrameBufF(void *topaddr, int bufferwidth, int pixelformat, int sync)
{
	unsigned int k1;
    k1 = SetK1(0);
	if(enabled && (topaddr) && (sceKernelPollSema(scrsema, 1) == 0)) sceKernelSetEventFlag(screvent, 1);
	SetK1(k1);
	DisplaySetFrameBufferInternal(2,topaddr, bufferwidth, pixelformat, sync);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// HOOKING FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void firstprepar(void)
{
int intc = pspSdkDisableInterrupts();
	// first of all we need modify sceGeEdramGetSize
    
	int *sceSetSize_p = (int *)((int)GeEdramGetSize); 
	int *fakeSetSize_p = (int *)((int)sceGeEdramGetSizeF); 

	sceSetSize_p[ 0] = 0x08000000 | ( ((((int)fakeSetSize_p)+0) & 0x0FFFFFFF ) >> 2 );
	sceSetSize_p[ 1] = 0x00000000;

	sceKernelIcacheInvalidateAll();

	// now let's modify sceDisplaySetFrameBuf
	sceSetSize_p = (int *)((int)DisplaySetFrameBuf);
	fakeSetSize_p = (int *)((int)sceDisplaySetFrameBufF); 

	sceSetSize_p[ 0] = 0x08000000 | ( ((((int)fakeSetSize_p)+0) & 0x0FFFFFFF ) >> 2 );
	sceSetSize_p[ 1] = 0x00000000;

	sceKernelIcacheInvalidateAll();

	sceGeEdramSetSize(4*1024*1024);

	pspSdkEnableInterrupts(intc);

	ready = 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void enableHooks(void)
{
	sceDisplayGetFrameBufferInternal(0,&address0,NULL,NULL,1);
	sceDisplayGetFrameBuf(&address2,&bufwidth2,&psm2,0);
	int intc = pspSdkDisableInterrupts();
	
	/**/
	
	int *p = (int *)((int)DisplaySetMode);
	int *Save =(int *)SetModeF;
	Save[0] = p[0]; Save[1] = p[1];
	int *p2 = (int *)((int)SetMode);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	
	Save[2] = 0x08000000 | ( ((((int)p)+8) & 0x0FFFFFFF ) >> 2 );
	
	sceKernelIcacheInvalidateAll();

	int * sceSetSize_p = (int *)((int)DisplaySetFrameBufferInternal);
	int * fakeSetSize_p = (int *)((int)DisplaySetFrameBufferInternalF);
	
	int * modSetSize_p = (int *)((int)setFrmBufInternal);

	modSetSize_p[0] = sceSetSize_p[ 0];
	modSetSize_p[1] = sceSetSize_p[ 1];
	
	sceSetSize_p[ 0] = 0x08000000 | ( ((((int)fakeSetSize_p)+0) & 0x0FFFFFFF ) >> 2 );
	sceSetSize_p[ 1] = 0x00000000;

	modSetSize_p[2] = 0x08000000 | ( ((((int)sceSetSize_p)+8) & 0x0FFFFFFF ) >> 2 );
    
	sceKernelIcacheInvalidateAll();
	
	pspSdkEnableInterrupts(intc);
	
    int psm = (psm2 == 3) ? 0 : psm2;
    DisplaySetFrameBufferInternal(0,(void*)0x04300000,768,psm,1);

    enabled = 1;
    
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void disableHooks(void)
{
    int intc = pspSdkDisableInterrupts();

	int * sceSetSize_p = (int *)((int)DisplaySetFrameBufferInternal);
	int * fakeSetSize_p = (int *)((int)setFrmBufInternal);
	
	sceSetSize_p[ 0] = fakeSetSize_p[0];
	sceSetSize_p[ 1] = fakeSetSize_p[1];

	sceKernelIcacheInvalidateAll();
	/**/
	int *p = (int *)((int)DisplaySetMode);
	int *Save =(int *)SetModeF;
	p[0] = Save[0]; p[1] = Save[1];
	
	sceKernelIcacheInvalidateAll();
    
	pspSdkEnableInterrupts(intc);

	enabled = 0;
	refresh = 1;
    
   return;
}
