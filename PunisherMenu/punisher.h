#pragma once
// Punisher

struct CVector {
	float x, y, z;
};

struct CQuat {
	float x, y, z, w;
};


struct Camera {
	CVector camPos;
	int pad;
	CQuat camRot;
	static Camera& TheCamera;
};

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

