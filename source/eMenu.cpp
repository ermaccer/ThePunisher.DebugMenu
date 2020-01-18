#include "eMenu.h"
#include "punisher.h"
#include "stdafx.h"
#include "calling.h"
#include <vector>
std::string vSkins[100];
std::vector<MenuFunction> menuEntries;
int DisplayMenu = 0;
char buffer[512];
int  MenuPosition = 0;
int  CurrentPage = 0;
int  CurrentSkin = 0;
int  ReadSkins = 0;
static int timer = GetTickCount();


std::string GetTrueFalse(int val)
{
	std::string ret;

	if (val == 0)  ret = "false";
	if (val == 1)  ret = "true";

	return ret;
}


std::string GetCharTrueFalse(char val)
{
	std::string ret;

	if (val == 0)  ret = "false";
	if (val == 1)  ret = "true";

	return ret;
}



void PopulateSkins()
{
	ReadSkins = 0;

	for (int i = 0; i < 100; i++)
		vSkins[i] = "";

	// read vpp buffer
	for (int i = 0; i < 15000; i++)
	{
		vpp_entry vpp = *(vpp_entry*)(0x8FFBD0 + (sizeof(vpp_entry) * i));

		if (!strlen(vpp.filename))
		{
			break;
		}
		char* filename = vpp.filename;

		if((filename[strlen(filename) - 3] == 'r' &&
		filename[strlen(filename) - 2] == 'x'&&
		filename[strlen(filename) - 1] == 'c'))
		{
			vSkins[ReadSkins] = vpp.filename;
			ReadSkins++;
		}

	}
}

void ToggleMenu()
{
	if (KeyHit(VK_F1))
	{
		if (GetTickCount() - timer <= 100) return;
		timer = GetTickCount();
		DisplayMenu ^= 1;

	}
}

void ProcessMenu()
{
	ToggleMenu();

	if (DisplayMenu)
	{
		RenderMenu();
		ControlMenu();
	}
	
}

void RenderMenu()
{
	if (DisplayMenu)
	{
		for (int i = 0; i < menuEntries.size(); i++)
		{
			int number = i;
			// add arrow
			std::string entry;
			if (MenuPosition == i) entry.append("->");
			else entry.append("  ");

			entry.append(menuEntries[number].name);

			// true false
			if (menuEntries[number].toggle == true)
			{
				entry.append(" = ");
				if (menuEntries[number].isChar)
					entry.append(GetCharTrueFalse(*menuEntries[number].addr_small));
				else entry.append(GetTrueFalse(*menuEntries[number].addr));
			}

			else if (menuEntries[number].swap == true)
			{
				entry.append(" : ");
				entry.append(vSkins[CurrentSkin]);
			}

			WriteLine(i, entry.c_str());

		}

		// draw info

		sprintf(buffer, "Current level file: %s", (char*)0x75841C);
		DrawGameText(0x8000, 0x10, buffer, -1, 98, 7);
		WriteLine(menuEntries.size() + 2, "The Punisher menu by ermaccer");
	}
}

void ControlMenu()
{
	if (KeyHit(VK_UP))
	{
		if (GetTickCount() - timer <= 100) return;
		timer = GetTickCount();
		MenuPosition--;
	}
	if (KeyHit(VK_DOWN))
	{
		if (GetTickCount() - timer <= 100) return;
		timer = GetTickCount();
		MenuPosition++;
	}


	if (KeyHit(VK_LEFT))
	{
		if (GetTickCount() - timer <= 100) return;
		timer = GetTickCount();
		CurrentSkin--;
	}
	if (KeyHit(VK_RIGHT))
	{
		if (GetTickCount() - timer <= 100) return;
		timer = GetTickCount();
		CurrentSkin++;
	}


	if (MenuPosition + 1 > menuEntries.size()) MenuPosition = 0 ;
	if (MenuPosition < 0) MenuPosition = menuEntries.size() - 1;
	if (CurrentSkin - 1 < 0) CurrentSkin = ReadSkins;
	if (CurrentSkin + 1 > ReadSkins) CurrentSkin = 0;


	if (KeyHit(VK_RETURN))
	{
		int number = MenuPosition;

		if (!menuEntries[number].toggle)
		{
			if (menuEntries[number].hasfunction)
				menuEntries[number].func();
			else if (menuEntries[number].swap)
				SetPlayerModel(vSkins[CurrentSkin].c_str());
		}

		if (menuEntries[number].toggle)
		{
			if (GetTickCount() - timer <= 200) return;
			timer = GetTickCount();
			if (menuEntries[number].isChar)
				*menuEntries[number].addr_small ^= 1;
			else *menuEntries[number].addr ^= 1;

		}
	}
}

void AddFunction(std::string name, std::function<void()> func, bool toggle, int * addr, bool has_func, char * addr_sm, bool is_char, std::string model, bool bswap)
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
	menfunc.swap = bswap;
	menuEntries.push_back(menfunc);
}

void CallFunction(std::string name, std::function<void()> func)
{
	int temp;
	AddFunction(name, func, 0, &temp, true, 0,false,"none",false);
}

void ChangePlayerModel(std::string name)
{
	int temp;
	std::function<void()> tempf;
	AddFunction(name, tempf, 0, &temp, false, 0, false,"none",true);
}

void ToggleValue(std::string name, int * value)
{
	std::function<void()> dummy;
	AddFunction(name, dummy, true, value, false, "none", false,"none",false);
}

void ToggleValueChar(std::string name, char* value)
{
	std::function<void()> dummy;
	AddFunction(name, dummy, true, 0, false, value, true, "none", false);
}

void HookMenu()
{
	ToggleMenu();
	if (DisplayMenu)
	{
		RenderMenu();
		ControlMenu();
	}
	Call<0x53BA80>();
}

// similar thing to mh2 writedebug
void WriteLine(int line, const char * string)
{
	SetDrawTextColor(0,0,0,255);
	DrawGameText(0x28 + 2, 12 * line + 13, string, 2, 98, 12);
	SetDrawTextColor(255, 255, 255, 255);
	DrawGameText(0x28, 12 * line + 12, string, 2, 98, 12);
}
