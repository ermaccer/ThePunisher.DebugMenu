#pragma once
// Punisher Menu
//
//


#define ICON_COLT 0
#define ICON_6SHOOTER 0x0F
#define ICON_DEAGLE 0x11

#define PTR_COLT 0xC817
#define PTR_6SHOOTER 0xF832
#define PTR_DEAGLE 0x9836


struct vpp_entry {
	char        filename[24] = {};
	int         rawSize;
	int         size;
};


struct weapon_struct
{
	short weapon_ptr;
	short pad;
	int icon;
	int unk;
	int ammo;
};


bool KeyHit(unsigned int keyCode);


// pun functions
int GetCurrentLanguage();
int LoadVolitionPackage(const char* packageName);
void SetDrawTextColor(int red, int green, int blue, int alpha);
int  DrawGameText(int x, int y,const  char* text, int font, int style, int unk);

// menu functions

void SetPlayerModel(const char* name);
void GiveScore();
void RestorePlayerModel();
void LoadLevel();