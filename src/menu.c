#include "menu.h"
#include <pspinit.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////

extern SceCtrlData pad,oldpad;
extern SceUID scrthid;
extern char tv, spb, fullscreen, fps_lim, pri;
extern int vcount,vcount2;
extern SceUInt SCREEN_WAIT_TIMEOUT;
extern FusaConfig config;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN MENU
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void menu(void)
{
memset((void*)0x44300000, 0, 0xBCA00);

	char disp = 1, pos = 0;
    
	while(disp)
	{
	sceCtrlPeekBufferPositive(&pad, 1);
		
		char val[40];
		int * te = (int*)sceHibari_driver_1F7D879D;
		sprintf(val,"0x%x 0x%x",(int)sceHibari_driver_1F7D879D,te[0]);
		
		blit_string(XX,YY-SZ,val,1);
		
		blit_string(XX,YY,str_1,1);
		
		int cable = pspDveMgrCheckVideoOut();
        
		switch (cable) {
		case 1: blit_string(XX,YY+SZ,str_2,1);
			break;
		case 2: blit_string(XX,YY+SZ,str_3,1);
		    break;
		default:
		     	blit_string(XX,YY+SZ,str_4,1);
		};
		            char i;
		            for (i = 2; i < 10; i++) {
		            if (pos == i-2 ) blit_string(XX,YY+SZ*i,str_e,1); else blit_string(XX,YY+SZ*i,str_f,1);
					};
		            
                    blit_string(XX+OFS*3,YY+SZ*2,str_5,1);
		   if (!tv) blit_string(XX+OFS*3,YY+SZ*3,str_6,1); else blit_string(XX+OFS*3,YY+SZ*3,str_7,1);
	       if (spb) blit_string(XX+OFS*3,YY+SZ*4,str_8,1); else blit_string(XX+OFS*3,YY+SZ*4,str_9,1);
	                
	                char buf[40];
	                
	                sprintf(buf,str_10,100-pri);
	                blit_string(XX+OFS*3,YY+SZ*5,buf,1);
	                sprintf(buf,str_11,fps_lim);
	                blit_string(XX+OFS*3,YY+SZ*6,buf,1);
	                sprintf(buf,str_12,fullscreen);
                    blit_string(XX+OFS*3,YY+SZ*7,buf,1);
                    
	       	        blit_string(XX+OFS*3,YY+SZ*8,str_13,1);
	             	blit_string(XX+OFS*3,YY+SZ*9,str_14,1);

	             	blit_string(XX,YY+SZ*10,CPRTS,1);
                    
                    blit_string(XX+OFS*2,YY+SZ*2+pos*SZ,">",1);
		if (pad.Buttons)
		{
		 if (pad.Buttons & PSP_CTRL_UP && !(oldpad.Buttons & PSP_CTRL_UP) ) {
		 if (pos == 0) pos = 7; else pos--;
		 }
         
		 if (pad.Buttons & PSP_CTRL_DOWN && !(oldpad.Buttons & PSP_CTRL_DOWN) ) {
		 if (pos == 7) pos = 0; else pos++;
		 }
         
		 if (pad.Buttons & PSP_CTRL_CIRCLE ) 
		 {
		 //////////////////////////////// EXIT COMMAND
		 blit_string(XX+OFS*2,YY+SZ*2+pos*SZ," ",1);
		 blit_string(XX+OFS*2,YY+SZ*2+7*SZ,">",1);
		 
		 SCREEN_WAIT_TIMEOUT = (1000000/fps_lim);
		 sceKernelChangeThreadPriority (scrthid, pri);
		 
		 sceKernelDelayThread(100*1000);
		 disp = 0;
		 /////////////////////////////////////////////
		 }
		};
		
		if (pad.Buttons & PSP_CTRL_RIGHT ){
		////////////////////////////////////////////// <> commands
		switch (pos) {
		case 3:
		//////////////////////////////////////////////// PRIORITY
		if (pri < 2) pri = 99; else pri--;
		if (pad.Buttons & PSP_CTRL_RTRIGGER) { if (pri < 11) pri = 99; else pri -= 9; };
		sceKernelDelayThread(200000);
		/////////////////////////////////////////////////////////
		break;
	    case 4:
		///////////////////////////////////////////////////// FPS
		if (fps_lim > 98) fps_lim = 1 ; else fps_lim++;
		if (pad.Buttons & PSP_CTRL_RTRIGGER) { if (fps_lim > 90) fps_lim = 1; else fps_lim+=9; };
		sceKernelDelayThread(200000);
		/////////////////////////////////////////////////////////
		break;
	    case 5:
		///////////////////////////////////////////////////// FSM
		if (!(oldpad.Buttons & PSP_CTRL_RIGHT)){
		if (fullscreen == 3) fullscreen = 3; else fullscreen++;
		}
		/////////////////////////////////////////////////////////
		break;
		};
		////////////////////////////////////// end of <> commands
		};
		
		if (pad.Buttons & PSP_CTRL_LEFT ){
		////////////////////////////////////////////// <> commands
		switch (pos) {
		case 3:
		//////////////////////////////////////////////// PRIORITY
		if (pri > 99) pri = 1;	else pri++;
		if (pad.Buttons & PSP_CTRL_RTRIGGER) { if (pri > 90) pri = 1; else pri += 9; };
		sceKernelDelayThread(200000);
		/////////////////////////////////////////////////////////
		break;
	    case 4:
		///////////////////////////////////////////////////// FPS
		if (fps_lim < 2) {fps_lim = 99;} else fps_lim--;
		if (pad.Buttons & PSP_CTRL_RTRIGGER) { if (fps_lim < 10) fps_lim = 99; else fps_lim -= 9; };
		sceKernelDelayThread(200000);
		/////////////////////////////////////////////////////////
		break;
	    case 5:
		///////////////////////////////////////////////////// FSM
		if (!(oldpad.Buttons & PSP_CTRL_LEFT)) {
		if (fullscreen == 0) fullscreen = 0; else fullscreen--;
		}
		/////////////////////////////////////////////////////////
		break;
		};
		////////////////////////////////////// end of <> commands
		};
		
		
		 if (pad.Buttons & PSP_CTRL_CROSS && !(oldpad.Buttons & PSP_CTRL_CROSS) ){
		/////////////////////////////////////// COMMANDS HANDLING
		switch (pos) {
		case 0:
		
		////////////////////////////////////////// ENABLE/DISABLE
		plugin_E_D();
		//if (spb)  speedboost_disable();
		if (spb) { speedboost_disable(); };
		disp = 0;
        /////////////////////////////////////////////////////////
        
		break;
		case 1:
		
		////////////////////////////////////////////////// TV/LCD
        switchtoTV_LCD(cable);
        disp = 0;
		/////////////////////////////////////////////////////////
		
		break;
		case 2:
		
		///////////////////////////////////////////////////// SPB
			if (!spb) {
			speedboost_enable();
			//disp=0;
			} else if (!(sceKernelInitKeyConfig() == 0x300)) {
			speedboost_disable();
			//disp=0;
			};
		/////////////////////////////////////////////////////////
		
		break;
		case 6:
	    
		/////////////////////////////////////// SCREEN ADJUSTMENT
		while (pad.Buttons & PSP_CTRL_CROSS ) sceCtrlPeekBufferPositive(&pad, 1);
        settings();
	    /////////////////////////////////////////////////////////
	    
		break;
		case 7:
		
		//////////////////////////////////////////////////// EXIT
	    SCREEN_WAIT_TIMEOUT = (1000000/fps_lim);
		sceKernelChangeThreadPriority (scrthid, pri);
		disp = 0;
		/////////////////////////////////////////////////////////
		
		break;
		};
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		oldpad = pad;

    }
		sceKernelDelayThread(100000);
	
		while (pad.Buttons & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS))
		{
		sceCtrlPeekBufferPositive(&pad, 1);
		}
	memset((void*)0x44300000, 0, 0xBCA00);
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADDITIONAL SETTINGS
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void settings(void)
{
memset((void*)0x44300000, 0, 0xBCA00);
char disp = 1, pos = 0;

	while(disp)
	{
	sceCtrlPeekBufferPositive(&pad, 1);
	
	blit_string(XX,YY,str_15,1);
	char i;
    for (i = 1; i < 4; i++) {
    if (pos == i-1 ) blit_string(XX,YY+SZ*i,str_e,1); else blit_string(XX,YY+SZ*i,str_f,1);
	};
	blit_string(XX+OFS*3,YY+SZ*1,str_17,1);
	blit_string(XX+OFS*3,YY+SZ*2,str_18,1);
	blit_string(XX+OFS*3,YY+SZ*3,str_16,1);
	blit_string(XX,YY+SZ*4,CPRTS,1);
	blit_string(XX+OFS*2,YY+SZ+pos*SZ,">",1);
if (pad.Buttons)
{
	if (pad.Buttons & PSP_CTRL_UP && !(oldpad.Buttons & PSP_CTRL_UP) ) {
	if (pos == 0) pos = 2; else pos--;
	}

	if (pad.Buttons & PSP_CTRL_DOWN && !(oldpad.Buttons & PSP_CTRL_DOWN) ) {
	if (pos == 2) pos = 0; else pos++;
	}
	
	if (pad.Buttons & PSP_CTRL_CIRCLE ) 
	{
	blit_string(XX+OFS*2,YY+SZ+pos*SZ," ",1);
	blit_string(XX+OFS*2,YY+SZ*3,">",1);

	sceKernelDelayThread(100*1000);
	disp = 0;
	}
	if (pad.Buttons & PSP_CTRL_CROSS ) 
	switch (pos) {
	case 0:
	//BUTTONS
	while (pad.Buttons & PSP_CTRL_CROSS ) sceCtrlPeekBufferPositive(&pad, 1);
	buttonsassignment();
	break;
	case 1:
	//SCREEN
	adjustscreen();
	break;
	case 2:
		disp = 0;
	}
}
	oldpad = pad;
	sceKernelDelayThread(10000);
	}
		while (pad.Buttons & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS))
		{
		sceCtrlPeekBufferPositive(&pad, 1);
		}
	
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// BUTTONS ASSIGNMENT
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void buttonsassignment()
{
char disp = 1, pos = 0, r = 1;
memset((void*)0x44300000, 0, 0xBCA00);
while(disp)
	{
	sceCtrlPeekBufferPositive(&pad, 1);
	    blit_string(XX,YY,str_41,1);
#define spaces "                                         "
	if (r) {switch (pos)
	{
	case 0:
		blit_string(XX,YY+SZ*2,str_42,1);
	    blit_string(XX,YY+SZ*4,spaces,1);
	    showbuttons(config.menu,YY+SZ*4);
		break;
	case 1:
		blit_string(XX,YY+SZ*2,str_43,1);
	    blit_string(XX,YY+SZ*4,spaces,1);
	    showbuttons(config.tvout,YY+SZ*4);
		break;
	case 2:
		blit_string(XX,YY+SZ*2,str_44,1);
	    blit_string(XX,YY+SZ*4,spaces,1);
		showbuttons(config.spb,YY+SZ*4);
		break;
	case 3:
		blit_string(XX,YY+SZ*2,str_45,1);
	    blit_string(XX,YY+SZ*4,spaces,1);
		showbuttons(config.switcher,YY+SZ*4);
	};
	r=0;}
	
	    if (pad.Buttons)
	    {
		if (pad.Buttons & PSP_CTRL_LEFT && !(oldpad.Buttons & PSP_CTRL_LEFT) )
		{if (pos == 0) pos = 3; else pos--; r=1;}
		
		if (pad.Buttons & PSP_CTRL_RIGHT && !(oldpad.Buttons & PSP_CTRL_RIGHT) )
		{if (pos == 3) pos = 0; else pos++; r=1;}
		
		if (pad.Buttons & PSP_CTRL_CIRCLE ) {
		disp = 0;
		savesettings();
		while (pad.Buttons & PSP_CTRL_CIRCLE) sceCtrlPeekBufferPositive(&pad, 1);
		}
		
		if (pad.Buttons & PSP_CTRL_CROSS ) {
		while (pad.Buttons & PSP_CTRL_CROSS) sceCtrlPeekBufferPositive(&pad, 1);
		
		int sec = 3;
		char buf[40];
		
		for (sec=3;sec > 0;sec--) {
		sprintf(buf,str_46,sec);
		blit_string(XX,YY+SZ*3,buf,1);
		blit_string(XX,YY+SZ*4,spaces,1);
		sceCtrlPeekBufferPositive(&pad, 1);
		showbuttons(pad.Buttons,YY+SZ*4);
		sceKernelDelayThread(1000000);
		}

		refreshconfig(pos,pad.Buttons);
		blit_string(XX,YY+SZ*3,spaces,1);
		
		while (pad.Buttons & (PSP_CTRL_CIRCLE | PSP_CTRL_CROSS)) sceCtrlPeekBufferPositive(&pad, 1);
		
		}
		
		}
		
	oldpad = pad;
	sceKernelDelayThread(10000);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// SCREEN ADJUSTMENT
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void adjustscreen()
{
u16 * ofs; u16 * _ofs; int y;
    memset((void*)0x44300000, 0, 0xBCA00);
	switch (fullscreen)
	{
	case 0:
		ofs = (u16*)config.offset0;
		_ofs = ofs;
		
		blit_string(XX,YY,str_19,1);
		
		for (y = 0 ; y < (768*239); y+=768) {ofs[y+288]=0xFFFF; ofs[y+288+480]=0xFFFF; ofs[y+288+0x31200]=0xFFFF; ofs[y+288+480+0x31200]=0xFFFF;}
	    
	    while (!(pad.Buttons & PSP_CTRL_CIRCLE)) {
		    sceCtrlPeekBufferPositive(&pad, 1);
		    
		    if ((pad.Buttons & PSP_CTRL_RIGHT)) {if ((int)(ofs + 8) < (std_ofs0+0xF0)) ofs+=8; else ofs = (u16*)(std_ofs0+0xF0);}
		    if ((pad.Buttons & PSP_CTRL_LEFT))  {if ((int)(ofs - 8) > (std_ofs0-0xF0)) ofs-=8; else ofs = (u16*)(std_ofs0-0xF0);}
    		
    if ((int)(_ofs) != (int)(ofs)) {
	
		for (y = 0 ;y < (768*239); y+=768) {
		
		_ofs[y+288]=0x0;   _ofs[y+288+480]=0x0;   _ofs[y+288+0x31200]=0x0;   _ofs[y+288+480+0x31200]=0x0;
		ofs[y+288]=0xFFFF; ofs[y+288+480]=0xFFFF; ofs[y+288+0x31200]=0xFFFF; ofs[y+288+480+0x31200]=0xFFFF;
		
		};
	
	blit_string(XX,YY,str_19,1);
	_ofs = ofs;
	}
			
		    sceKernelDelayThread(100000);
		    oldpad = pad;
	    }

	config.offset0 = (int)ofs;

	break;
	case 1:
		ofs = (u16*)config.offset1;
	   _ofs = ofs;
	
		blit_string(XX,YY,str_20,1);
	
	for (y = 0 ;y < 480; y++) {
	ofs[y+288] = 0xFFFF;
	ofs[y+288+0x31200] = 0xFFFF;
	};
	
	for (y = 0 ;y < (768*216); y+=768){
		ofs[y+288]=0xFFFF; ofs[y+288+480]=0xFFFF; ofs[y+288+0x31200]=0xFFFF; ofs[y+288+480+0x31200]=0xFFFF;
		};
	
	for (y = (768*216) ; y < 480+(768*216); y++) {
	ofs[y+288] = 0xFFFF;
	ofs[y+288+0x31200] = 0xFFFF;
	};
	
	    while (!(pad.Buttons & PSP_CTRL_CIRCLE)) {
		    sceCtrlPeekBufferPositive(&pad, 1);
		    {
		    if ((pad.Buttons & PSP_CTRL_RIGHT)) {if ((int)(ofs + 8) < (std_ofs0-0xF0+288+(23*768*2)) ) ofs+=8; else ofs = (u16*)(std_ofs0-0xF0+288+(23*768*2));}
		    if ((pad.Buttons & PSP_CTRL_LEFT))  {if ((int)(ofs - 8) > (std_ofs0-0xF0) ) ofs-=8; else ofs = (u16*)(std_ofs0-0xF0);}
		 
		    if ((pad.Buttons & PSP_CTRL_UP))   {if ((int)(ofs - 768) > (std_ofs0-0xF0))                ofs-=(768); else ofs = (u16*)(std_ofs0-0xF0);}
		    if ((pad.Buttons & PSP_CTRL_DOWN)) {if ((int)(ofs + 768) < (std_ofs0-0xF0+288+(23*768*2))) ofs+=(768); else ofs = (u16*)(std_ofs0-0xF0+288+(23*768*2));}
			};
    if ((int)(_ofs) != (int)(ofs)) {
	
	for (y = 288 ;y < 480+288; y++) {
	_ofs[y+(768*216)] = 0x0;
	_ofs[y+0x31200+(768*216)] = 0x0;
	_ofs[y] = 0x0;
	_ofs[y+0x31200] = 0x0;
	};
	
	    for (y = 288 ;y < 288+(768*216); y+=768){
		_ofs[y]=0x0;   _ofs[y+480]=0x0;   _ofs[y+0x31200]=0x0;   _ofs[y+480+0x31200]=0x0;
		};
		
		for (y = 288 ;y < 288+(768*216); y+=768){
		ofs[y]=0xFFFF; ofs[y+480]=0xFFFF; ofs[y+0x31200]=0xFFFF; ofs[y+480+0x31200]=0xFFFF;
		};
	
	for (y = 288 ;y < 480+288; y++) {
	ofs[y+(768*216)] = 0xFFFF;
	ofs[y+0x31200+(768*216)] = 0xFFFF;
	ofs[y] = 0xFFFF;
	ofs[y+0x31200] = 0xFFFF;
	};
	
    blit_string(XX,YY,str_20,1);
	_ofs = ofs;
	}
			
		    sceKernelDelayThread(200000);
		    oldpad = pad;
	    }
		
	    config.offset1 = (int)ofs;
	break;
	case 2:
	    ofs = (u16*)config.offset2;
	   _ofs = ofs;
	
		blit_string(XX,YY,str_20,1);
	
	for (y = 0 ;y < 671; y++) {
	ofs[y+96] = 0xFFFF;
	ofs[y+96+0x31200] = 0xFFFF;
	};
	
	for (y = 0 ;y < (768*216); y+=768){
		ofs[y+96]=0xFFFF; ofs[y+96+672]=0xFFFF; ofs[y+96+0x31200]=0xFFFF; ofs[y+96+672+0x31200]=0xFFFF;
		};
	
	for (y = (768*216) ; y < 480+(768*216); y++) {
	ofs[y+96] = 0xFFFF;
	ofs[y+96+0x31200] = 0xFFFF;
	};
	
	    while (!(pad.Buttons & PSP_CTRL_CIRCLE)) {
		    sceCtrlPeekBufferPositive(&pad, 1);
		    {
		    if ((pad.Buttons & PSP_CTRL_RIGHT)) {if ((int)(ofs + 1) < (0x44300000-(96*2)+96+(23*768*2)) ) ofs++; else ofs = (u16*)(0x44300000-(96*2)+96+(23*768*2));}
		    if ((pad.Buttons & PSP_CTRL_LEFT))  {if ((int)(ofs - 1) > (0x44300000-(96*2)) ) ofs--; else ofs = (u16*)(0x44300000-(96*2));}
		 
		    if ((pad.Buttons & PSP_CTRL_UP))   {if ((int)(ofs - 768) > (0x44300000-(96*2)) ) ofs-=(768); else ofs = (u16*)(0x44300000-(96*2));}
		    if ((pad.Buttons & PSP_CTRL_DOWN)) {if ((int)(ofs + 768) < (0x44300000-(96*2)+96+(23*768*2))) ofs+=(768); else ofs = (u16*)(0x44300000-(96*2)+96+(23*768*2));}
			};
    if ((int)(_ofs) != (int)(ofs)) {
	
	for (y = 96 ;y < 672+96; y++) {
	_ofs[y+(768*216)] = 0x0;
	_ofs[y+0x31200+(768*216)] = 0x0;
	_ofs[y] = 0x0;
	_ofs[y+0x31200] = 0x0;
	};
	
	    for (y = 96 ;y < 96+(768*216); y+=768){
		_ofs[y]=0x0;   _ofs[y+672]=0x0;   _ofs[y+0x31200]=0x0;   _ofs[y+672+0x31200]=0x0;
		};
		
		for (y = 96 ;y < 96+(768*216); y+=768){
		ofs[y]=0xFFFF; ofs[y+672]=0xFFFF; ofs[y+0x31200]=0xFFFF; ofs[y+672+0x31200]=0xFFFF;
		};
	
	for (y = 96 ;y < 672+96; y++) {
	ofs[y+(768*216)] = 0xFFFF;
	ofs[y+0x31200+(768*216)] = 0xFFFF;
	ofs[y] = 0xFFFF;
	ofs[y+0x31200] = 0xFFFF;
	};
	
    blit_string(XX,YY,str_20,1);
	_ofs = ofs;
	}
			
		    sceKernelDelayThread(200000);
		    oldpad = pad;
	    }
		
	    config.offset2 = (int)ofs;
	}
	savesettings();
	memset((void*)0x44300000, 0, 0xBCA00);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void showbuttons(int key, int y)
{
key &= ~(PSP_CTRL_WLAN_UP|PSP_CTRL_REMOTE|PSP_CTRL_DISC|PSP_CTRL_MS);

if (key){
char buf[200];
char buf2[200];
sprintf(buf,str_21);

#define str_str "%s"
#define add_2_str(a) sprintf(buf,a,buf2); sprintf(buf2,str_str,buf);

if (key & PSP_CTRL_SELECT)    add_2_str(str_22);
if (key & PSP_CTRL_START)     add_2_str(str_23);
if (key & PSP_CTRL_UP)        add_2_str(str_24);
if (key & PSP_CTRL_RIGHT)     add_2_str(str_25);
if (key & PSP_CTRL_DOWN)      add_2_str(str_26);
if (key & PSP_CTRL_LEFT)      add_2_str(str_27);
if (key & PSP_CTRL_LTRIGGER)  add_2_str(str_28);
if (key & PSP_CTRL_RTRIGGER)  add_2_str(str_29);
if (key & PSP_CTRL_TRIANGLE)  add_2_str(str_30);
if (key & PSP_CTRL_CIRCLE)    add_2_str(str_31);
if (key & PSP_CTRL_CROSS)     add_2_str(str_32);
if (key & PSP_CTRL_SQUARE)    add_2_str(str_33);
if (key & PSP_CTRL_HOME)      add_2_str(str_34);
if (key & PSP_CTRL_HOLD)      add_2_str(str_35);
if (key & PSP_CTRL_NOTE)      add_2_str(str_36);
if (key & PSP_CTRL_SCREEN)    add_2_str(str_37);
if (key & PSP_CTRL_VOLUP)     add_2_str(str_38);
if (key & PSP_CTRL_VOLDOWN)   add_2_str(str_39);

blit_string(XX,y,buf,1);
} else 
blit_string(XX,y,str_40,1);
}
