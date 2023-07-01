#include <pspkernel.h>
#include <systemctrl.h>
	
#define MAX_THREAD	64
	
typedef struct THREAD{
	int count;
	SceUID pThread;
} THREAD;

int count_Start;
SceUID Thread_Start[MAX_THREAD];
THREAD bufNow;

int nkLoad(){

	count_Start = 0;

	sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, Thread_Start, MAX_THREAD, &count_Start);

	bufNow.count = 0;
	bufNow.pThread = 0;
	
	return 0;
}

int nkThreadSuspend(SceUID thId){

	int i, j;
	SceUID myThread, *Thread_Now;

	if(bufNow.pThread != 0) return 1;

	bufNow.pThread = sceKernelAllocPartitionMemory(1, "th", 0, MAX_THREAD*sizeof(SceUID), NULL);
	if(bufNow.pThread < 0){
		bufNow.count = 0;
		bufNow.pThread = 0;
		return 1;
	}
	Thread_Now = (SceUID*)sceKernelGetBlockHeadAddr(bufNow.pThread);

	sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, Thread_Now, MAX_THREAD, &(bufNow.count));
	myThread = sceKernelGetThreadId();

	for(i = 0; i < bufNow.count; i++){
		unsigned char match = 0;
		SceUID tmp_thid = Thread_Now[i];
		
		for(j = 0; j < count_Start; j++){
			if((tmp_thid == Thread_Start[j]) || (tmp_thid == thId) || (tmp_thid == myThread)){
				match = 1;
				j = count_Start;
			}
		}
		if(match == 0){
			sceKernelSuspendThread(tmp_thid);
		}
	}

	return 0;
}

int nkThreadResume(SceUID thId){

	int i, j;
	SceUID myThread, *Thread_Now;

	if(bufNow.pThread == 0) return 1;

	Thread_Now = (SceUID*)sceKernelGetBlockHeadAddr(bufNow.pThread);
	myThread = sceKernelGetThreadId();
	

	for(i = 0; i < bufNow.count; i++){
		unsigned char match = 0;
		SceUID tmp_thid = Thread_Now[i];
		
		for(j = 0; j < count_Start; j++){
			if((tmp_thid == Thread_Start[j]) || (tmp_thid == thId) || (tmp_thid == myThread)){
				match = 1;
				j = count_Start;
			}
		}

		if(match == 0){
			sceKernelResumeThread(tmp_thid);
		}
	}
	
	sceKernelFreePartitionMemory(bufNow.pThread);
	bufNow.count = 0;
	bufNow.pThread = 0;

	return 0;
}
