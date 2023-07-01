#include <pspdisplay.h>
#include <pspsdk.h>

#include "local.h"

#ifdef TUR
extern unsigned char msx[];
#endif

#ifdef ENG
extern unsigned char msx[];
#endif

#ifdef GER
extern unsigned char msx[];
#endif

#ifdef ITA
#include "itafont.h"
#endif

#ifdef RUS
#include "rufont.h"
#endif

#ifdef DUT
extern unsigned char msx[];
#endif

#ifdef POL
#include "polfont.h"
#endif

#ifdef BRA
extern unsigned char msx[];
#endif
	
#ifdef HEB
#include "hebfont.h"
#endif

#ifdef HUN
extern unsigned char msx[];
#endif

#ifdef FRE
extern unsigned char msx[];
#endif

#ifdef TUR
extern unsigned char msx[];
#endif

#ifdef ESP
extern unsigned char msx[];
#endif

#ifdef PT
extern unsigned char msx[];
#endif

extern char enabled;
extern void* address2;
extern int bufwidth2, psm2;

static int bufferwidth;
static int pixelformat;
static void * vram;
static int mode;


int font_init(void) {
	
	if (enabled) {
	vram = (void *)0x44300000;
    bufferwidth = 768;
    pixelformat = 1;
    mode = pixelformat;
	return pixelformat;
	};

	sceDisplayGetFrameBuf(&vram, &bufferwidth, &pixelformat, 0);
	vram = (void *)(((unsigned long)vram) | 0x40000000);
	
    if(bufferwidth == 0) return -1;
    
	// Check for valid Pixelformat
	if((pixelformat == PSP_DISPLAY_PIXEL_FORMAT_565) || (pixelformat == PSP_DISPLAY_PIXEL_FORMAT_5551) || (pixelformat == PSP_DISPLAY_PIXEL_FORMAT_4444) || (pixelformat == PSP_DISPLAY_PIXEL_FORMAT_8888)) {
		mode = pixelformat;
		return pixelformat;
	}

	return -1;

}

int blit_string(int sx,int sy,const char *msg,int bg) {
	font_init();
	// Kill Process if Bufferwidth aint playing along...
	if(bufferwidth == 0) return -1;

	// Needed Variables
	int p = 0;
	int x = 0;
	int y = 0;
	int ox = sx;

	// Decide what to do... 16bit? 32bit?...
	if(mode == PSP_DISPLAY_PIXEL_FORMAT_8888) {
		// Convert Pointer to 32bit
		unsigned int * vram32 = (unsigned int *)vram;

		// Get to work...
		for(x = 0; msg[x]; x++) {
			u8 code = msg[x];// & 0x7f;
			for(y = 0; y < 8; y ++) {
				int offset = (sy + y) * bufferwidth + ox;
				u8 pmap = msx[code * 8 + y];
				for(p = 0; p < 6; p ++) {
					if(pmap & 0x80) {
						
						vram32[offset] = 0x00FFFFFF;
						vram32[offset+1] = 0x00FFFFFF;
						if (enabled) 
						{
							vram32[offset+0x31200] =  0x00FFFFFF;
							vram32[offset+0x31201] =  0x00FFFFFF;
						}
					
					} else if(bg == 1)
					{
						
						vram32[offset] = 0x00000000;
						vram32[offset+1] = 0x00000000;
						if (enabled) 
						{
							vram32[offset+0x31200] =  0x00000000;
							vram32[offset+0x31201] =  0x00000000;
						}
					}
					pmap <<= 1;
					offset += 2;
				}
			}
			ox += 12;
		}
	} else if((pixelformat == PSP_DISPLAY_PIXEL_FORMAT_565) || (pixelformat == PSP_DISPLAY_PIXEL_FORMAT_5551) || (pixelformat == PSP_DISPLAY_PIXEL_FORMAT_4444)) {
		// Convert Pointer to 16bit
		unsigned short * vram16 = (unsigned short *)vram;

		// Get to work...
		for(x = 0; msg[x]; x++) {
			u8 code = msg[x];// & 0x7f;
			for(y = 0; y < 8; y ++) {
				int offset = (sy + y) * bufferwidth + ox;
				u8 pmap = msx[code * 8 + y];
				for(p = 0; p < 6; p ++) {
					if(pmap & 0x80) {
						
						vram16[offset] = 0xFFFF;
						vram16[offset+1] = 0xFFFF;
						if (enabled) 
						{
							vram16[offset+0x31200] =  0xFFFF;
							vram16[offset+0x31201] =  0xFFFF;
						}
						
					} else if( bg == 1 ) {
						
						vram16[offset] = 0x0000;
						vram16[offset+1] = 0x0000;
						
						if (enabled) 
						{
							vram16[offset+0x31200] =  0x0000;
							vram16[offset+0x31201] =  0x0000;
						}
					    
					}
					pmap <<= 1;
					offset += 2;
				}
			}
			ox += 12;
		}
	}
	
	return x;
}
