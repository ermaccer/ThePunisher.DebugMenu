#include "eMenu.h"
#include "punisher.h"
#include "MemoryMgr.h"
#include "core.h"
#include <vector>
#include <Windows.h>
#include "eMain.h"
#include "IniReader.h"

using namespace Memory::VP;

eMenu TheMenu;

char buffer[512];

int& ShowWeaponsOnly = *(int*)0x7085F4;
char& ParticleDebugTwo = *(char*)0x6A174A;
char& ParticleDebugOne = *(char*)0x6A1749;
char& SoundsPlaying = *(char*)0xE6087A;
char& DisplayPlayerMovement = *(char*)0x70E23C;
char& BlackWhiteFilter = *(char*)0x70E231;

float& TimeStep = *(float*)0x6769FC;
std::string GetTrueFalse(int value)
{
	return value ? "true" : "false";
}


void HookMenu()
{
	TheMenu.ProcessMenu();
	Call<0x53BA80>();
}

// similar thing to mh2 writedebug
void WriteLine(int line, const char * string)
{
	SetDrawTextColor(0,0,0,255);
	DrawGameText(0x28 + 2, 12 * line + 13, string, 0, 98, 12);
	SetDrawTextColor(255, 255, 255, 255);
	DrawGameText(0x28, 12 * line + 12, string, 0, 98, 12);
}

void PopulateSkins()
{
	TheMenu.ReadSkins = 0;
	for (int i = 0; i < 100; i++)
		TheMenu.vSkins[i] = "";

	// read vpp buffer
	for (int i = 0; i < 15000; i++)
	{
		vpp_entry vpp = *(vpp_entry*)(0x8FFBD0 + (sizeof(vpp_entry) * i));

		if (!strlen(vpp.filename))
			break;

		char* filename = vpp.filename;

		if ((filename[strlen(filename) - 3] == 'r' &&
			filename[strlen(filename) - 2] == 'x' &&
			filename[strlen(filename) - 1] == 'c'))
		{
			TheMenu.vSkins[TheMenu.ReadSkins] = vpp.filename;
			TheMenu.ReadSkins++;
		}

	}
}

void SetSlowMotion()
{
	TimeStep = 2.0f;
}

void SetFastMotion()
{
	TimeStep = 0.5f;
}

void ResetSpeed()
{
	TimeStep = 1.0f;
}

void dummy()
{
}

void eMenu::PreInitialize()
{
	CIniReader ini("");

	KeyEnableMenu = ini.ReadInteger("Settings", "keyMenuEnable", VK_F1);
	KeyMenuExecute = ini.ReadInteger("Settings", "keyMenuExecute", VK_RETURN);
	KeyMenuGoBack = ini.ReadInteger("Settings", "keyMenuBack", VK_BACK);
	KeyMenuItemDOWN = ini.ReadInteger("Settings", "keyMenuDown", VK_DOWN);
	KeyMenuItemUP = ini.ReadInteger("Settings", "keyMenuUp", VK_UP);
	KeyMenuItemLEFT = ini.ReadInteger("Settings", "keyMenuLeft", VK_LEFT);
	KeyMenuItemRIGHT = ini.ReadInteger("Settings", "keyMenuRight", VK_RIGHT);

	iFreeCameraKeyXPlus = ini.ReadInteger("Settings", "iFreeCameraKeyXPlus", 0);
	iFreeCameraKeyXMinus = ini.ReadInteger("Settings", "iFreeCameraKeyXMinus", 0);


	iFreeCameraKeyYPlus = ini.ReadInteger("Settings", "iFreeCameraKeyYPlus", 0);
	iFreeCameraKeyYMinus = ini.ReadInteger("Settings", "iFreeCameraKeyYMinus", 0);

	iFreeCameraKeyZPlus = ini.ReadInteger("Settings", "iFreeCameraKeyZPlus", 0);
	iFreeCameraKeyZMinus = ini.ReadInteger("Settings", "iFreeCameraKeyZMinus", 0);


}

void eMenu::Initialize()
{
	PreInitialize();
	AddFunctionEntry("Restore Player Model", RestorePlayerModel);
	AddFunctionEntry("Load Player Models", PopulateSkins);
	AddPlayerModelEntry("Change Player Model To");
	AddFunctionEntry("Give Score", GiveScore);
	AddFunctionEntry("Slowmotion", SetSlowMotion);
	AddFunctionEntry("Normal Speed", ResetSpeed);
	AddFunctionEntry("Speed Up", SetFastMotion);

	AddToggleIntEntry("Infinite Ammo", &m_infiniteAmmo);
	AddToggleIntEntry("Infinite Health", &m_infiniteHealth);
	AddToggleIntEntry("Free Camera", &m_freeCamera);
	AddToggleIntEntry("Freeze Camera", &m_freezeCamera);

	AddToggleCharEntry("B/W Filter", &BlackWhiteFilter);

	AddFunctionEntry("====Original Debug Functions====",dummy);

	AddToggleIntEntry("DEBUG: Show Weapons Only", &ShowWeaponsOnly);
	AddToggleCharEntry("DEBUG: Show Particle Source", &ParticleDebugTwo);
	AddToggleCharEntry("DEBUG: Disable Particles", &ParticleDebugOne);
	AddToggleCharEntry("DEBUG: Display Sounds Playing", &SoundsPlaying);
	AddToggleCharEntry("DEBUG: Display Player Movement Mode", &DisplayPlayerMovement);
	AddFunctionEntry("================================", dummy);
}

void eMenu::OnKeyLeft()
{
	CurrentSkin--;
}

void eMenu::OnKeyRight()
{
	CurrentSkin++;
}

void eMenu::OnKeyDown()
{
	iCurrentItem++;
}

void eMenu::OnKeyUp()
{
	iCurrentItem--;
}

void eMenu::OnKeyExecute()
{
	int number = iCurrentItem;

	if (!menuEntries[number].toggle)
	{
		if (menuEntries[number].hasfunction)
			menuEntries[number].func();
		else if (menuEntries[number].swap)
			SetPlayerModel(vSkins[CurrentSkin].c_str());
	}

	if (menuEntries[number].toggle)
	{
		if (menuEntries[number].isChar)
			*menuEntries[number].addr_small ^= 1;
		else *menuEntries[number].addr ^= 1;

	}
}

void eMenu::OnKeyToggle()
{
	m_active ^= 1;
}

void eMenu::ProcessMenu()
{
	if (m_infiniteAmmo)
		Nop(0x5055A5, 3);
	else
	{
		Patch<char>(0x5055A5, 0xFF);
		Patch<char>(0x5055A5 + 1, 0x4E);
		Patch<char>(0x5055A5 + 2, 0x30);
	}

	if (m_infiniteHealth)
	{
		Nop(0x420D49, 6);
	}
	else
	{
		Patch<char>(0x420D49, 0xD9);
		Patch<char>(0x420D49 + 1, 0x96);
		Patch<char>(0x420D49 + 2, 0x88);
		Patch<char>(0x420D49 + 3, 0x02);
		Patch<char>(0x420D49 + 4, 0x00);
		Patch<char>(0x420D49 + 5, 0x00);
	}

	if (m_freezeCamera)
	{
		Nop(0x406ACC, 5);
	}
	else
	{
		Patch<char>(0x406ACC, 0xE8);
		Patch<int>(0x406ACC + 1, 0xFFFFE1FF);
	}
	if (m_freeCamera)
	{
		Nop(0x405A60, 2);
		Nop(0x405A66, 3);
		Nop(0x405A78, 3);
	}
	else
	{

		Patch<char>(0x405A60, 0x89);
		Patch<char>(0x405A60 + 1, 0x02);

		Patch<char>(0x405A66, 0x89);
		Patch<char>(0x405A66 + 1, 0x4A);
		Patch<char>(0x405A66 + 2, 0x04);

		Patch<char>(0x405A78, 0x89);
		Patch<char>(0x405A78 + 1, 0x42);
		Patch<char>(0x405A78 + 2, 0x08);

	}

	if (m_freeCamera)
	{


		if (GetAsyncKeyState(iFreeCameraKeyXPlus))
			Camera::TheCamera.camPos.x += m_fFreeCameraSpeed;
		if (GetAsyncKeyState(iFreeCameraKeyXMinus))
			Camera::TheCamera.camPos.x -= m_fFreeCameraSpeed;
		if (GetAsyncKeyState(iFreeCameraKeyYPlus))
			Camera::TheCamera.camPos.y += m_fFreeCameraSpeed;
		if (GetAsyncKeyState(iFreeCameraKeyYMinus))
			Camera::TheCamera.camPos.y -= m_fFreeCameraSpeed;
		if (GetAsyncKeyState(iFreeCameraKeyZPlus))
			Camera::TheCamera.camPos.z += m_fFreeCameraSpeed;
		if (GetAsyncKeyState(iFreeCameraKeyZMinus))
			Camera::TheCamera.camPos.z -= m_fFreeCameraSpeed;
	}


	if (m_active)
	{
		DrawMenu();
		ProcessControls();
	}

}

void eMenu::DrawMenu()
{
	for (int i = 0; i < menuEntries.size(); i++)
	{
		int number = i;
		std::string entry;

		entry.append(menuEntries[number].name);

		// true false
		if (menuEntries[number].toggle)
		{
			entry.append(" = ");
			if (menuEntries[number].isChar)
				entry.append(GetTrueFalse(*menuEntries[number].addr_small));
			else entry.append(GetTrueFalse(*menuEntries[number].addr));
		}
		else if (menuEntries[number].swap)
		{
			entry.append(" = ");
			entry.append(vSkins[CurrentSkin]);
		}

		SetDrawTextColor(0, 0, 0, 255);
		DrawGameText(0x28 + 2, 12 * i + 13, entry.c_str(), 0, 98, 12);
		if (iCurrentItem == i)
			SetDrawTextColor(244,55,55, 255);
		else
			SetDrawTextColor(255, 255, 255, 255);
		DrawGameText(0x28, 12 * i + 12, entry.c_str(), 0, 98, 12);

	}

	// draw info

	sprintf(buffer, "Current level file: %s", (char*)0x75841C);
	SetDrawTextColor(0, 0, 0, 255);
	DrawGameText(0x8000 + 0x20, 0x10 + 0x11, buffer, 0, 98, 12);
	SetDrawTextColor(255, 255, 255, 255);
	DrawGameText(0x8000, 0x10, buffer, 0, 98, 7);



	if (m_freeCamera)
	{
		sprintf(buffer, "Camera Pos: %f %f %f ", Camera::TheCamera.camPos.x, Camera::TheCamera.camPos.y, Camera::TheCamera.camPos.z);
		WriteLine(menuEntries.size() + 2, buffer);
	}



	WriteLine(menuEntries.size() + 4, "The Punisher menu by ermaccer");
}

void eMenu::ProcessControls()
{
	if (iCurrentItem + 1 > menuEntries.size()) iCurrentItem = 0;
	if (iCurrentItem < 0) iCurrentItem = menuEntries.size() - 1;
	if (CurrentSkin - 1 < 0) CurrentSkin = ReadSkins;
	if (CurrentSkin + 1 > ReadSkins) CurrentSkin = 0;

}

void eMenu::AddItem(std::string name, std::function<void()> func, bool toggle, int* addr, bool has_func, char* addr_sm, bool is_char, std::string model, bool swap)
{
	MenuFunction menfunc;
	menfunc.name = name;
	menfunc.func = func;
	menfunc.toggle = toggle;
	menfunc.addr = addr;
	menfunc.hasfunction = has_func;
	menfunc.addr_small = addr_sm;
	menfunc.isChar = is_char;
	menfunc.plrmodel = model;
	menfunc.swap = swap;
	menuEntries.push_back(menfunc);
}

void eMenu::AddFunctionEntry(std::string name, std::function<void()> func)
{
	int temp;
	AddItem(name, func, 0, &temp, true, 0, false, "none", false);
}

void eMenu::AddPlayerModelEntry(std::string name)
{
	int temp;
	std::function<void()> tempf;
	AddItem(name, tempf, 0, &temp, false, 0, false, "none", true);
}

void eMenu::AddToggleIntEntry(std::string name, int* value)
{
	std::function<void()> dummy;
	AddItem(name, dummy, true, value, false, "none", false, "none", false);
}

void eMenu::AddToggleCharEntry(std::string name, char* value)
{
	std::function<void()> dummy;
	AddItem(name, dummy, true, 0, false, value, true, "none", false);
}
