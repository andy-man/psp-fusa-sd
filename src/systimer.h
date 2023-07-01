#ifndef __SYSTIMER__
#define __SYSTIMER__

#ifdef __cplusplus
extern "C" {
#endif

typedef int SceSysTimerId;
// 3.7x C99073E3
// 3.80 3289FE8F
SceSysTimerId (*STimerAlloc)(void);
// 3.7x C105CF38
// 3.80 24FE4B16
void (*STimerFree)(SceSysTimerId timer);
// 3.7x A95143E2
// 3.80 30F59F2D
void (*STimerStartCount)(SceSysTimerId timer);
// 3.7x 4A01F9D3
// 3.80 76F20D5C
void (*STimerStopCount)(SceSysTimerId timer);
// 3.7x 54BB5DB4
// 3.80 32327068
void (*STimerResetCount)(SceSysTimerId timer);
// 3.7x 228EDAE4
// 3.80 33C2A3F7
void (*STimerGetCount)(SceSysTimerId timer, int* count);
// 3.7x 975D8E84
// 3.80 9757BF80
void (*STimerSetHandler)(SceSysTimerId timer, int cycle, int (*handler)(void), int unk1);
// 3.7x 53231A15
// 3.80 5CA94AF4
void (*STimerForKernel_53231A15)(SceSysTimerId timer, int unk1);
// 3.7x B53534B4
// 3.80 F13611FE
void (*STimerForKernel_B53534B4)(SceSysTimerId timer, int unk1, int unk2);

#ifdef __cplusplus
}
#endif

#endif
