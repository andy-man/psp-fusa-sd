#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsysmem_kernel.h>
#include "thrdsmgr.h"
#include "menu.h"
#include "FS/fsmodes.h"
#include "spb.h"
#define GET_JUMP_TARGET(x) (0x80000000 | (((x) & 0x03FFFFFF) << 2))
PSP_MODULE_INFO("FuSa", 0x1000, 1, 50);
PSP_MAIN_THREAD_ATTR(0);
PSP_MAIN_THREAD_STACK_SIZE_KB(0);



void getfunctions(void);

/////////////////////////////// VARIABLES

FusaConfig config;

SceUID main_thid;

SceUID scrthid = -1;
SceUID scrsema = -1;
SceUID screvent = -1;

SceCtrlData pad,oldpad;

SceUInt SCREEN_WAIT_TIMEOUT = 100*1000;
char fps_lim = 10;
char pri = 24;

char enabled = 0, fullscreen = 3, tv = 0, refresh = 0, spb = 0, ready = 0;
char lcd = 0, running = 1;

void *address2,*address0;
int	bufwidth2, psm2, bufwidth0, psm0;

int NIDS3[] = {0x3289FE8F, 0x30F59F2D, 0x9757BF80, 0x24FE4B16, 0x33C2A3F7, 0x32327068, 0x76F20D5C};
int NIDS4[] = {0xC2A3128E, 0xB59C9521, 0xED89221E, 0xBA8872BE, 0xC7CAD056, 0xFD3E3BAF, 0x979481B3};
int NIDS5[] = {0x1E99FA82, 0xD9A211BC, 0xF5A719D1, 0x1013EB8B, 0xF9B7ABB1, 0x0AB3BC9C, 0x62AB11D2};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// SCREEn THREAD
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int screen_thread(SceSize args, void *argp)
{
// Create sema
	scrsema = sceKernelCreateSema("ScreenSema", 0, 1, 1, NULL);
	if(scrsema < 0)
	{
		sceKernelExitDeleteThread(0);
	};
// Create event
	screvent = sceKernelCreateEventFlag("ScreenEvent", 0, 0, NULL);
	if(screvent < 0)
	{
		sceKernelExitDeleteThread(0);
	};

    

	while(enabled)
	{
		
		int ret;
		u32 status;
		SceUInt timeout;

		timeout = SCREEN_WAIT_TIMEOUT; 
		ret = sceKernelWaitEventFlag(screvent, 3, PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR, &status, &timeout);
		if((ret < 0) && (ret != SCE_KERNEL_ERROR_WAIT_TIMEOUT))
		{
			/* DISABLE HOOKS etc */
			disableHooks();
			
			sceKernelExitDeleteThread(0);
		}

		if((status & 1) || (ret == SCE_KERNEL_ERROR_WAIT_TIMEOUT))
		{

unsigned int k1;
k1 = SetK1(0);

/*
_sw( 0xFFFFFFFF, 0xBC00000C );
*/

switch (fullscreen) {
case 0:
	if (address0) {
	copy16to16(address0 + 0x4000,(void*)config.offset0);
	} else {
	sceDisplayGetFrameBuf(&address2,&bufwidth2,&psm2,0);
	if (bufwidth2 == 1024) {
	copy16to16_1024(address2 + 0x4000,(void*)config.offset0);
	} else {
	if (psm2 == 3) copy32to16(address2 + 0x8000,(void*)config.offset0); else copy16to16(address2 + 0x4000,(void*)config.offset0);};
	};
break;

case 1:
	if (address0) {
	copy16to16f1(address0 - (512*2),(void*)config.offset1 );
	} else {
	sceDisplayGetFrameBuf(&address2,&bufwidth2,&psm2,0);
	if (bufwidth2 == 1024) {
	copy16to16_1024f1(address2 - (1024*2),(void*)config.offset1);
	} else {
	if (psm2 == 3) copy32to16f1(address2 - (512*4),(void*)config.offset1); else copy16to16f1(address2 - (512*2),(void*)config.offset1 );};
	};
break;

case 2:
	if (address0) {
	copy16to16f2(address0 - (512*2),(void*)config.offset2 );
	} else {
	sceDisplayGetFrameBuf(&address2,&bufwidth2,&psm2,0);
	if (bufwidth2 == 1024) {
	copy16to16_1024f2(address2 - (1024*2),(void*)config.offset2);
	} else {
	if (psm2 == 3) copy32to16f2(address2 - (512*4),(void*)config.offset2); else copy16to16f2(address2 - (512*2),(void*)config.offset2 );};
	};
break;

case 3:
	if (address0) {
	copy16to16f3(address0,(void*)std_ofs3);
	} else {
	sceDisplayGetFrameBuf(&address2,&bufwidth2,&psm2,0);
	if (bufwidth2 == 1024) {
	copy16to16_1024f3(address2,(void*)std_ofs3);
	} else {
	if (psm2 == 3) copy32to16f3(address2,(void*)std_ofs3); else copy16to16f3(address2,(void*)std_ofs3);};
	};
break;
};

SetK1(k1);
			if(ret != SCE_KERNEL_ERROR_WAIT_TIMEOUT)
			{
				sceKernelSignalSema(scrsema, 1);
			}
		}

		if(status & 2)
		{
			sceKernelSleepThread();
		}
		
	}

	sceKernelExitDeleteThread(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN THREAD
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int threadMain(SceSize args, void *argp)
{

    //sceKernelDelayThread(10*1000*1000); // wait 10 sec
    if (loadsettings() < 0) {loadstdconfig(); savesettings();}
	getfunctions();
	sceKernelDelayThread(1000*1000);
	
while(running){

    sceCtrlPeekBufferPositive(&pad, 1);
    
		if  (refresh){
		sceKernelDelayThread(100*1000);
		DisplaySetFrameBufferInternal(0,address0,512,1,1);
		sceDisplayGetFrameBufferInternal(0,&address0,NULL,NULL,1);
		if (!address0) {
		sceKernelDelayThread(50*1000);
		DisplaySetFrameBufferInternal(2,address2,bufwidth2,psm2,1);
		};
		
		refresh = 0;
		};

			if ( (pad.Buttons & config.menu) == config.menu ) 
			{
			while ( (pad.Buttons & config.menu) == config.menu )  sceCtrlPeekBufferPositive(&pad, 1);
			nkThreadSuspend(sceKernelGetThreadId());
			sceKernelDelayThread(100*1000);
			
			if (!enabled) plugin_E_D();
			sceKernelSetEventFlag(screvent, 2);
			
			menu();
			
			nkThreadResume(sceKernelGetThreadId());
			
			sceKernelWakeupThread(scrthid);
			}
            
            if (enabled && ((pad.Buttons & config.tvout) == config.tvout))
			{
			switchtoTV_LCD(pspDveMgrCheckVideoOut());
			}
            
            if ( ((pad.Buttons & config.spb) == config.spb)) //enabled &&
			{
			sceKernelSetEventFlag(screvent, 2);
				if (!spb) {
				speedboost_enable();
				} else speedboost_disable();
				
				while ( (pad.Buttons & config.spb) == config.spb ) sceCtrlPeekBufferPositive(&pad, 1);
			sceKernelWakeupThread(scrthid);
			}
			if ((pad.Buttons & config.switcher) == config.switcher)
			{
			nkThreadSuspend(sceKernelGetThreadId());
			sceKernelDelayThread(100*1000);
			plugin_E_D();
			sceKernelSetEventFlag(screvent, 2);
			sceKernelDelayThread(100*1000);
			nkThreadResume(sceKernelGetThreadId());
			sceKernelWakeupThread(scrthid);
			
			while ( (pad.Buttons & config.switcher) == config.switcher ) sceCtrlPeekBufferPositive(&pad, 1);
			}
			
	oldpad = pad;
	sceKernelDelayThread(15*1000);
	
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int module_start(SceSize args, void *argp)
{
	
	nkLoad();
	
	main_thid = sceKernelCreateThread("FuSa_SD", threadMain, 32, 0x800, 0, NULL);
	if (main_thid >= 0) sceKernelStartThread(main_thid, args, argp);
    
    return 0;
    
	sceKernelExitDeleteThread(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void exitplugin(void)
{
  if (spb) speedboost_disable();
  if (enabled) {
  plugin_E_D();
  running = 0;
  sceKernelWaitThreadEnd(main_thid, 0);
  };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int module_stop(SceSize args, void *argp)
{
	exitplugin();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int module_reboot_phase() 
{ 
  exitplugin();
  return 0; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void switchtoTV_LCD(int mode)
{
	    lcd = 1;
		if (tv && (pspDveMgrSetVideoOut( 0, 0, 480, 272, 0, 0, 0 ) == 0)) 
		{
		tv = 0;
		} else {
		if ((mode == 1) && (pspDveMgrSetVideoOut(2, 0x1d1, 480, 503, 1, 15, 0) == 0) ) tv = 1;
		if ((mode == 2) && (pspDveMgrSetVideoOut(0, 0x1d1, 480, 503, 1, 15, 0) == 0) ) tv = 1;
		};
		
		return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void plugin_E_D(void)
{
	    if (!enabled){
	        if (!ready) firstprepar();
	    	memset((void*)0x44300000, 0, 0xBCA00);

			enableHooks();
			switchtoTV_LCD(pspDveMgrCheckVideoOut());
			//POPS
			if (sceKernelInitKeyConfig() == 0x300) 
			{
			fps_lim = 40;
			SCREEN_WAIT_TIMEOUT = (1000000/40);
			pri = 12;
			} else {
			fps_lim = 10;
			SCREEN_WAIT_TIMEOUT = (1000000/10);
			pri = 24;
			}
			
		    scrthid = sceKernelCreateThread("ScreenThread", screen_thread, pri, 0x800, PSP_THREAD_ATTR_VFPU, NULL);
			if(scrthid >= 0) 
			{
				sceKernelStartThread(scrthid, 0, NULL);
			};
			
	    } else {
	    	/* DISABLE HOOKS etc */
	    	if (tv) switchtoTV_LCD(0);
	    	sceKernelDelayThread(100*1000);
    		disableHooks();
    		sceKernelDelayThread(100*1000);
	    };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void getfunctions(void)
{
char i = 1;
while (i) {
    sceKernelDelayThread(100*1000);
    // let's find all necessary functions
    GeEdramGetSize = (void*)sctrlHENFindFunction("sceGE_Manager", "sceGe_driver", 0x1F6752AD);
    if (GeEdramGetSize == NULL) continue;
    
    DisplaySetFrameBuf = (void*)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x289D82FE);
    if (DisplaySetFrameBuf == NULL) continue;
    addr_SetFrameBuf = (int)DisplaySetFrameBuf;
    
	addr_GetFrameBuf = (int)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0xEEDA2E54);
    
    DisplaySetFrameBufferInternal = (void*)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay_driver", 0x63E22A26);
	if (DisplaySetFrameBufferInternal == NULL) continue;
	addr_SetFrameBufInt = (int)DisplaySetFrameBufferInternal;
	
	DisplaySetMode = (void*)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0x0E20F177);
	if (DisplaySetMode == NULL) continue;
	
	addr_SetMode = sctrlHENFindFunction("IRSHELL", "irshell", 0x253FD506);
	
	sceHprm_driver_1528D408 = (void*)sctrlHENFindFunction("sceHP_Remote_Driver", "sceHprm_driver", 0x1528D408);
	if (sceHprm_driver_1528D408 == NULL) continue;
	
	///////////////////
	sceHibari_driver_1F7D879D = (void*)sctrlHENFindFunction("sceDisplay_Service", "sceHibari_driver", 0x1F7D879D);
	if (sceHibari_driver_1F7D879D == NULL) continue;
	sceHibari_driver_1F7D879D = (void*)((int)sceHibari_driver_1F7D879D - 0xDC8);
	///////////////////
	
	sceImposeSetVideoOutMode = (void*)sctrlHENFindFunction("sceImpose_Driver", "sceImpose_driver", 0x116DDED6);
	if (sceImposeSetVideoOutMode == NULL) continue;
	
	pspVblankInterruptHandler = (void*)((int)sctrlHENFindFunction("sceDisplay_Service", "sceDisplay", 0xB4F378FA)+0x248);
	
	int* NIDS;
	char mod[18] = "sceDve_Driver";
	if (sceKernelDevkitVersion() < 0x04000000)
	{
		NIDS = &NIDS3[0];
	} else if (sceKernelDevkitVersion() > 0x04900000) {
		NIDS = &NIDS5[0];
		sprintf(mod,"sceDisplay_Service");
	} else {
		NIDS = &NIDS4[0];
	}
	
	sceDve_driver_DEB2F80C = (void*)sctrlHENFindFunction(mod, "sceDve_driver", 0xDEB2F80C);
	sceDve_driver_93828323 = (void*)sctrlHENFindFunction(mod, "sceDve_driver", 0x93828323);
	sceDve_driver_0B85524C = (void*)sctrlHENFindFunction(mod, "sceDve_driver", 0x0B85524C);
	sceDve_driver_A265B504 = (void*)sctrlHENFindFunction(mod, "sceDve_driver", 0xA265B504);
	
	if ((sceDve_driver_93828323 && sceDve_driver_A265B504 && sceDve_driver_0B85524C && sceDve_driver_DEB2F80C) == 0) continue;
	
	sceGeEdramSetSize = (void*)sctrlHENFindFunction("sceGE_Manager", "sceGe_driver", 0x5BAA5439);
	if (sceGeEdramSetSize == NULL) continue;
	
	STimerAlloc = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[0]);
	STimerStartCount = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[1]);
	STimerSetHandler = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[2]);
	STimerFree = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[3]);
	STimerGetCount  = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[4]);
	STimerResetCount  = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[5]);
	STimerStopCount = (void*)sctrlHENFindFunction("sceSystimer", "SysTimerForKernel", NIDS[6]);

	
	i = 0;
   }
}