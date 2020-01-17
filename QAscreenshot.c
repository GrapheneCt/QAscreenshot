#include <taihen.h>
#include <vitasdk.h>

static SceUID g_hooks[2];

void _start() __attribute__ ((weak, alias ("module_start")));

static tai_hook_ref_t ref_hook0;
static int sceRegMgrGetKeyInt_patched(const char* category, const char* name, int* buf) {
	int ret = TAI_CONTINUE(int, ref_hook0, category, name, buf);
	if (strcmp(name, "debug_screenshot_format") == 0 || strcmp(name, "debug_screenshot") == 0)
	{
		*buf = 1;
		return 0;
	}
	return ret;
}

static tai_hook_ref_t ref_hook1;
static int sceSblQafMgrIsAllowScreenShotAlways_patched(int* arg1, int* arg2, int* arg3, int* arg4) {
	return 1;
}

int module_start(SceSize argc, const void *args)
{
  g_hooks[0] = taiHookFunctionImport(&ref_hook0,
	TAI_MAIN_MODULE,
	0xC436F916, //SceRegMgr
	0x16DDF3DC,
	sceRegMgrGetKeyInt_patched);
  g_hooks[1] = taiHookFunctionImport(&ref_hook1,
	TAI_MAIN_MODULE,
	0x756B7E89, //SceSblQafMgr
	0xD22A8731,
	sceSblQafMgrIsAllowScreenShotAlways_patched);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) 
{
	if (g_hooks[0] >= 0) taiHookRelease(g_hooks[0], ref_hook0);
	if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], ref_hook1);
  return SCE_KERNEL_STOP_SUCCESS;
}
