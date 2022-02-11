#include <iostream>
#include "punisher.h"
#include "eMenu.h"
#include "MemoryMgr.h"
#include "eMain.h"

using namespace Memory::VP;


void Init()
{
	TheMenu.Initialize();
	Patch<int>(0x524EC6 + 4, (int)MainHooks::HookedWndProc);
	InjectHook(0x4DE65D, HookMenu);
}


bool CheckExecutable()
{
	char* name = (char*)(0x605AC0);

	if (strcmp(name, "VolitionPunisher") == 0)
		return true;
	else
	{
		MessageBoxA(0, "Invalid executable!", "PunisherMenu", MB_ICONINFORMATION);
		return false;
	}

}


extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		if (CheckExecutable())
		{
			Init();
		}
	}
}