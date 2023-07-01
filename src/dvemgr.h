int pspDveMgrCheckVideoOut();
int pspDveMgrSetVideoOut(int, int, int, int, int, int, int);

int (*sceHprm_driver_1528D408)();
int (*sceImposeSetVideoOutMode)(int, int, int);
int (*sceDve_driver_DEB2F80C)(int);
int (*sceDve_driver_93828323)(int);
int (*sceDve_driver_0B85524C)(int);
int (*sceDve_driver_A265B504)(int, int, int);
int (*sceDve_driver_0CC96C20)(int);
void (*sceHibari_driver_1F7D879D)(int);