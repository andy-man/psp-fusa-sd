#include <pspkernel.h>
#include <pspsysevent.h>
#include "dvemgr.h"
	
u32 SetK1(unsigned int k1);

#define RETURN(x) res = x; SetK1(k1); return x

int pspDveMgrCheckVideoOut()
{
	int k1 = SetK1(0);
	int intr = sceKernelCpuSuspendIntr();

	// Warning: nid changed between 3.60 and 3.71
	int cable = sceHprm_driver_1528D408();

	sceKernelCpuResumeIntr(intr);
	SetK1(k1);

	return cable;
}

int pspDveMgrSetVideoOut(int u, int mode, int width, int height, int x, int y, int z)
{
	int k1 = SetK1(0);
	int res;
	
	sceHibari_driver_1F7D879D(251);

	res = sceDve_driver_DEB2F80C(u);
	if (res < 0)
	{
		RETURN(-1);
	}

	// These params will end in sceDisplaySetMode
	res = sceDisplaySetMode(mode, width, height);
	if (res < 0)
	{
		RETURN(-2);
	}

	res = sceDve_driver_93828323(0);
	if (res < 0)
	{
		RETURN(-3);
	}

	res = sceDve_driver_0B85524C(1);
	if (res < 0)
	{
		RETURN(-4);
	}

	res = sceDve_driver_A265B504(x, y, z);
	if (res < 0)
	{
		RETURN(-5);
	}
	
	SetK1(k1);
	return res;
}
