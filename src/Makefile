TARGET = FuSa
OBJS =  irshell.o main.o thrdsmgr.o blit.o menu.o config.o \
	injected.o dve.o hook.o spb.o \
	fs/fs0.o fs/fs1.o fs/fs2_16b.o fs/fs2_32b.o fs/fs3.o \
	exports.o 
BUILD_PRX=1
PRX_EXPORTS=exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

INCDIR = ../inc
CFLAGS = -O2 -Os -G0 -Wall -fno-strict-aliasing -fno-pic -D_PSPSLIM -fno-builtin-printf
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../lib
LIBS = -lpspsystemctrl_kernel

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
