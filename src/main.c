/*
ECCHI PROJECT - PSV2000 ENABLE
TEAM IOWA:
Sys - Project manager, PSV2000 Testkit tester, and PSV2000 Testkit Ethernet Adapter
dots_tb - Head RE

With help from:
teakhanirons - I forgot what he did...
lyzzz - PSV2000 tester
CHΞCKΞR- Almost PSV2000 tester
realusagichan - PSV2000 tester, made a Y-cable
CelesteBlue - RE help
Princess of Sleeping - Pictures of PSV2000
SilicaAndPina - PSV1000 Devkit tests
Zexceil - for being interested in hardware manufacturing
*/

#include <dolcesdkkern.h>
#include <taihen.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define printf ksceDebugPrintf

static tai_hook_ref_t SceSysconForDriver_D6F6D472_ref;
SceUID hook = -1;

static SceUID SceSysconForDriver_D6F6D472_patched(uint32_t *pwr_val) {
	
	SceUID ret, state;
	
	ENTER_SYSCALL(state);

	ret = TAI_CONTINUE(SceUID, SceSysconForDriver_D6F6D472_ref, pwr_val);
	printf("SceSysconForDriver_D6F6D472_patched original %x\n", *pwr_val);
	if(*pwr_val == 0x700)
		printf("SceSysconForDriver_D6F6D472_patched new %x\n\n", *pwr_val = 0x200);
	EXIT_SYSCALL(state);
	return ret;
	
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){
	hook = taiHookFunctionImportForKernel(KERNEL_PID, &SceSysconForDriver_D6F6D472_ref, "SceUsbServ", TAI_ANY_LIBRARY, 0xD6F6D472, SceSysconForDriver_D6F6D472_patched);
	printf("SceSysconForDriver_D6F6D472_patched hook%x\n", hook);
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args){
	if(hook>-1)
		taiHookReleaseForKernel(hook, SceSysconForDriver_D6F6D472_ref);
	return SCE_KERNEL_STOP_SUCCESS;
}