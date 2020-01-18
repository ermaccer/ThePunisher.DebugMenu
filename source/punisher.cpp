#include "punisher.h"
#include "calling.h"
#include <iostream>
#include "stdafx.h"
#include <vector>

static int timer = GetTickCount();
bool bIsInLevel = false;
bool bDisplayText = true;

bool KeyHit(unsigned int keyCode)
{
	return (GetKeyState(keyCode) & 0x8000) != 0;
}

int GetCurrentLanguage()
{
	return CallAndReturn<int, 0x4DD520>();
}

int LoadVolitionPackage(const char * packageName)
{
	return CallAndReturn<int, 0x50EC20, const char*>(packageName);
}

int LoadGlobalVolitionPackage(const char * packageName, int a2, int buffer, int a4)
{
	return CallAndReturn<int, 0x50EE60, const char*, int ,int ,int>(packageName, a2, buffer, a4);
}

void SetDrawTextColor(int red, int green, int blue, int alpha)
{
	Call<0x510B60, int, int, int, int>(red,green,blue,alpha);
}

int DrawGameText(int x, int y, const  char* text, int font, int style, int unk)
{
	return CallAndReturn<int, 0x5186B0, int, int,const char*, int, int, int>(x,y,text,font,style,unk);
}

void SetPlayerModel(const char * name)
{

	char* filename = (char*)name;

	std::vector<vpp_entry> data;
	// read vpp buffer
	for (int i = 0; i < 15000; i++)
	{
		vpp_entry vpp = *(vpp_entry*)(0x8FFBD0 + (sizeof(vpp_entry) * i));
		if (!strlen(vpp.filename))
		{
			bDisplayText = true;
			break;
		}
		data.push_back(vpp);
	}
	
	// check if exists
	for (int i = 0; i < data.size(); i++)
	{
		if (strcmp(data[i].filename, filename) == 0)
		{
			bIsInLevel = true;
		}

	}
	// save model
	if (bIsInLevel)
	{
		//tshirt
		char* buffer = (char*)0x6B76C8;
		sprintf(buffer, filename);
		// trenchcoat
		buffer = (char*)0x6B73F8;
		sprintf(buffer, filename);
		// prison 
		buffer = (char*)0x6B7998;
		sprintf(buffer, filename);
		// jungle
		buffer = (char*)0x6B7C68;
		sprintf(buffer, filename);

		DrawGameText(0x8000, 0x50, "Restart section/level to apply!", 1, 98, 7);
	}
	else
	{
	   DrawGameText(0x8000, 0x50, "Model does not exist here!", 1, 98, 7);
	}

}

void GiveScore()
{
	Call<0x4BA600, int, int, int>(*(int*)0x7CF7EC,21, 1000);
}

void RestorePlayerModel()
{
	//tshirt
	char* buffer = (char*)0x6B76C8;
	sprintf(buffer, "punisher_tshirt.vcm");
	// trenchcoat
	buffer = (char*)0x6B73F8;
	sprintf(buffer, "punisher_trenchcoat.vcm");
	// prison 
	buffer = (char*)0x6B7998;
	sprintf(buffer, "punisher_prison.vcm");
	// jungle
	buffer = (char*)0x6B7C68;
	sprintf(buffer, "punisher_jungle.vcm");
}

