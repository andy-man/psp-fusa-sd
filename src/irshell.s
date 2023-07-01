.set noreorder

#include "pspstub.s"

STUB_START "irshell",0x00090000,0x00020005
STUB_FUNC 0x9C575BB0,irsGetPad
STUB_FUNC 0x9E272C4D,irsSwitchApp
STUB_FUNC 0x253FD506,irsFunc
STUB_END