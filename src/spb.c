#include "spb.h"
	
void irsGetPad(void);

DisplayVblank VBlank;
extern char spb;
SceSysTimerId vblanktmr;
int vcount = 0;
int vcount2 = 0;
int run = 0;
short interval[1000];

int vblevent = -1;
int vblankthid = -1;

SceUInt tvintsema = -1;
SceUInt tvintevent = -1;
int tvthid = -1;

int SetFrameBuf[10];
int GetFrameBuf[10];
int SetFrameBufInt[10];
int dSetMode[10];

int vblank_thread(SceSize args, void *argp)
{
// Create sema
	tvintsema = sceKernelCreateSema("TVINTSema", 0, 1, 1, NULL);
	if(tvintsema < 0)
	{
		sceKernelExitDeleteThread(0);
	};
// Create event
	tvintevent = sceKernelCreateEventFlag("TVINTEvent", 0, 0, NULL);
	if(tvintevent < 0)
	{
		sceKernelExitDeleteThread(0);
	};

	while(spb)
	{
		
		int ret;
		u32 status;
		SceUInt timeout;

		timeout = 100000; 
		ret = sceKernelWaitEventFlag(tvintevent, 3, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &status, &timeout);
		
		if((ret < 0) && (ret != SCE_KERNEL_ERROR_WAIT_TIMEOUT))
		{
			speedboost_disable();
			sceKernelExitDeleteThread(0);
		}

		if((status & 1) || (ret == SCE_KERNEL_ERROR_WAIT_TIMEOUT))
		{
			//TODO
			pspVblankInterruptHandler(0x1E);
			if(ret != SCE_KERNEL_ERROR_WAIT_TIMEOUT)
			{
				sceKernelSignalSema(tvintsema, 1);
			}
		}

		if(status & 2)
		{
			sceKernelSleepThread();
		}
		
	}
	
	sceKernelDeleteSema(tvintsema);
	sceKernelDeleteEventFlag(tvintevent);
	
	sceKernelExitDeleteThread(0);
	return 0;
}


int vblank_60Hz(SceSize args, void *argp) //SYSTIMER
{
	/*
	vcount++;
	if (vcount == 0xc24) {
	//sceKernelDisableSubIntr(PSP_VBLANK_INT, 0); vcount == 0xc24 //185a
	//run = 1;
	pspVblankInterruptHandler(0x1E);
	//run = 0;
	//sceKernelEnableSubIntr(PSP_VBLANK_INT, 0);
	vcount = 0;
	}
	*/
	vblevent = sceKernelCreateEventFlag("VblankEvent", 0, 0, NULL);
	if(vblevent < 0)
	{
		sceKernelExitDeleteThread(0);
	};
	
	while (spb) 
	{
		int ret;
		u32 status;
		SceUInt timeout;

		timeout = 100*1000; 
		ret = sceKernelWaitEventFlag(vblevent, 3, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &status, &timeout);
		
		if((status & 1) || (ret == SCE_KERNEL_ERROR_WAIT_TIMEOUT))
		{
		sceKernelDelayThread( 9*1000 );
		run = 1;
		pspVblankInterruptHandler(0x1E);
		run = 0;
		vcount++;
		};
	}
	
	return -1;
}

int vblank(int id) //VBLANK
{
	//
	//interval[run] = vcount;
	//vcount = 0;
	if(sceKernelPollSema(tvintsema, 1) == 0) sceKernelSetEventFlag(tvintevent, 1);
	return -1;
}

char g_data[0x400] __attribute__((aligned(64)));

void saveinfo(char * path)
{
	int fd;
	fd = sceIoOpen(path, PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	
	if(fd >= 0)
	{
		
	int i;
	//for (i = 0; i < 10; i++) SetFrameBuf[i] = _lw( addr_SetFrameBuf + i*4 );
	//for (i = 0; i < 10; i++) GetFrameBuf[i] = _lw( addr_GetFrameBuf + i*4 );
	//for (i = 0; i < 10; i++) SetFrameBufInt[i] = _lw( addr_SetFrameBufInt + i*4 );
	//for (i = 0; i < 10; i++) dSetMode[i] = _lw( (int)irsGetPad + i*4 );
		
		//sceIoWrite(fd, SetFrameBuf, sizeof(int)*10);
		//sceIoWrite(fd, GetFrameBuf, sizeof(int)*10);
		//sceIoWrite(fd, SetFrameBufInt, sizeof(int)*10);
		
		for (i = 0; i < 0x1000; i++) 
		{
			memcpy(g_data, (void*)(0x88000000+(0x400*i)), 0x400);
			sceIoWrite(fd, g_data, 0x400);
		}
		
		sceIoClose(fd);
		
	};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void speedboost_enable(void)
{
	
	saveinfo("ms0:/enablesbp.bin");
		
	//int intc = pspSdkDisableInterrupts();
	/*
	int *p,*p2,*Save =(int *)stuba;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x8EB9EC49); //sceDisplayWaitVblankCB
	Save[0] = p[0]; Save[1] = p[1];
	p2 = (int *)((int)waitvblank2);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x984C27E7); //sceDisplayWaitVblankStart
	Save[2] = p[0]; Save[3] = p[1];
	p2 = (int *)((int)waitvblank);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x36CDFADE); //sceDisplayWaitVblank
	Save[4] = p[0]; Save[5] = p[1];
	p2 = (int *)((int)waitvblank2);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x46F186C3); //sceDisplayWaitVblankStartCB
	Save[6] = p[0]; Save[7] = p[1];
	p2 = (int *)((int)waitvblank);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x9C6EAAD7); //sceDisplayGetVcount
	Save[8] = p[0]; Save[9] = p[1];
	p2 = (int *)((int)getvcount);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
*//*
	sceKernelIcacheInvalidateAll();
    
    p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x773DD3A3); //sceDisplayGetCurrentHcount
	Save[10] = p[0]; Save[11] = p[1];
	p2 = (int *)((int)gethcount);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;

	sceKernelIcacheInvalidateAll();
   
    p = (int *)sctrlHENFindFunction("sceThreadManager", "ThreadManForUser", 0xCEADEB47);//("sceController_Service", "sceCtrl", 0x1F803938); //sceCtrlReadBufferPositive
	Save[12] = p[0]; Save[13] = p[1];
	p2 = (int *)((int)CtrlReadBufferP);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	//p[3] = 0x00003021; //$a2 = 0

	sceKernelIcacheInvalidateAll();

	p = (int *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x60B81F86); //sceCtrlReadBufferNegative
	Save[14] = p[0]; Save[15] = p[1];
	p2 = (int *)((int)CtrlReadBufferN);
	p[ 0] = 0x08000000 | ( ((((int)p2)+0) & 0x0FFFFFFF ) >> 2 );
	p[ 1] = 0x00000000;
	//p[3] = 0x24060001; //$a2 = 1
	
	sceKernelIcacheInvalidateAll();
	
	VBlank.AccHcount = 0;
	VBlank.C = 0;
	VBlank.LineTotal = 0;
	VBlank.VblankEnd = 0;
	VBlank.evid = 0;
	SceUID evid = sceKernelCreateEventFlag("Vblank", 0x201, 0, 0);
	VBlank.evid = evid;
	*/
	
	//vblanktmr = STimerAlloc();
	//STimerSetHandler(vblanktmr, 0xFF, vblank_60Hz, 0); // ~1/60 sec
	//STimerStartCount(vblanktmr);
	spb = 1;
	/*
	vblankthid = sceKernelCreateThread("VblankThread", vblank_60Hz, 1, 0x800, 0, NULL);
	if(vblankthid >= 0) 
	{
		sceKernelStartThread(vblankthid, 0, NULL);
	};
	
	
	tvthid = sceKernelCreateThread("VBLThread", vblank_thread, 16, 0x800, 0, NULL);
	if(tvthid >= 0)
	{
		sceKernelStartThread(tvthid, 0, NULL);
	};
	
	sceKernelRegisterSubIntrHandler(PSP_VBLANK_INT, 0,  &vblank, 0);
	sceKernelEnableSubIntr(PSP_VBLANK_INT, 0);
	
	
	pspSdkEnableInterrupts(intc);*/
	//sceKernelDisableIntr(PSP_VBLANK_INT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void speedboost_disable(void)
{
	saveinfo("ms0:/disablesbp.bin");
	
	/*
	int intc = pspSdkDisableInterrupts();
	
	int *p,*Save =(int *)stuba;
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x8EB9EC49); //sceDisplayWaitVblankCB
	p[0] = Save[0]; p[1] = Save[1];
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x984C27E7); //sceDisplayWaitVblankStart
	p[0] = Save[2]; p[1] = Save[3];
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x36CDFADE); //sceDisplayWaitVblank
	p[0] = Save[4]; p[1] = Save[5];
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x46F186C3); //sceDisplayWaitVblankStartCB
	p[0] = Save[6]; p[1] = Save[7];
	
	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x9C6EAAD7); //sceDisplayGetVcount
	p[0] = Save[8]; p[1] = Save[9];

	p = (int *)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x773DD3A3); //sceDisplayGetCurrentHcount
	p[0] = Save[10]; p[1] = Save[11];

	p = (int *)sctrlHENFindFunction("sceThreadManager", "ThreadManForUser", 0xCEADEB47); //sceCtrlReadBufferPositive
	p[0] = Save[12]; p[1] = Save[13];
	//p[3] = 0x24060002; //$a2 = 2

	p = (int *)sctrlHENFindFunction("sceController_Service", "sceCtrl", 0x60B81F86); //sceCtrlReadBufferNegative
	p[0] = Save[14]; p[1] = Save[15];
	//p[3] = 0x24060003; //$a2 = 3
	
	sceKernelIcacheInvalidateAll();
  
	//STimerStopCount(vblanktmr);
  	//STimerFree(vblanktmr);
  	
  	sceKernelDisableSubIntr(PSP_VBLANK_INT, 0);
	sceKernelReleaseSubIntrHandler(PSP_VBLANK_INT, 0);
	*/
	spb = 0;
/*
	pspSdkEnableInterrupts(intc);
	//sceKernelEnableIntr(PSP_VBLANK_INT);
	
	int fd = sceIoOpen("ms0:/vblank.log", PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	
	if(fd >= 0)
	{
		
		for (run = 0; run < 1000; run++){
		char buf[10];
		sprintf(buf,"%x\n",interval[run]);
		sceIoWrite(fd, buf, strlen(buf) );
		}
		
		sceIoClose(fd);
	};
	*/
}
