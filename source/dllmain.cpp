#include "stdafx.h"
#include <iostream>
#include "punisher.h"
#include "eMenu.h"
#include "MemoryMgr.h"

using namespace Memory::VP;

int InfiniteAmmo;
// debug stuff?
int& ShowWeaponsOnly = *(int*)0x7085F4;
int ToggleParticleDebug;
char& ParticleDebugTwo = *(char*)0x6A174A;
char& ParticleDebugOne = *(char*)0x6A1749;
char& SoundsPlaying = *(char*)0xE6087A;
char& DisplayPlayerMovement = *(char*)0x70E23C;

void Init()
{
	Patch<int>(0x4DE65D + 1, (int)HookMenu - ((int)0x4DE65D + 5));
	CallFunction("Restore Player Model", RestorePlayerModel);
	CallFunction("Load Player Models", PopulateSkins);
	ChangePlayerModel("Change Player Model To:");
	CallFunction("Give Score", GiveScore);
	ToggleValue("Infinite Ammo", &InfiniteAmmo);
	ToggleValue("DEBUG: Show Weapons Only", &ShowWeaponsOnly);
	ToggleValueChar("DEBUG: Show Particle Source", &ParticleDebugTwo);
	ToggleValueChar("DEBUG: Disable Particles", &ParticleDebugOne);
	ToggleValueChar("DEBUG: Display Sounds Playing", &SoundsPlaying);
	ToggleValueChar("DEBUG: Display Player Movement Mode", &DisplayPlayerMovement);

	while (true)
	{
		if (InfiniteAmmo)
			Nop(0x5055A5, 3);
		else
		{
			Patch<char>(0x5055A5, 0xFF);
			Patch<char>(0x5055A5 + 1, 0x4E);
			Patch<char>(0x5055A5 + 2, 0x30);
		}
		Sleep(1);
	}
}


extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), nullptr, 0, nullptr);;
	}
}
