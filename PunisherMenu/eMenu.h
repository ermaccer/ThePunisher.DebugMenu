#pragma once
#include <string>
#include <functional>

#define MENU_PAGE_ENTRY 20
#define MENU_MAX_PAGES 2

struct Level {
	char name[24];
};


struct MenuFunction {
	std::string                       name;
	std::function<void()>             func;
	bool                              toggle;
	int*                              addr;
	char*                             addr_small;
	bool                              hasfunction;
	bool                              isChar;
	std::string                       plrmodel;
	bool                              swap;
};


class eMenu {
private:
	std::vector<MenuFunction> menuEntries;
	int  iCurrentItem = 0;
	int  iCurrentPage = 0;
	int  CurrentSkin = 0;

public:
	std::string vSkins[100];
	int  ReadSkins = 0;
	bool m_active = false;
	int m_freeCamera = false;
	int m_freezeCamera = false;
	int m_infiniteAmmo = false;
	int m_infiniteHealth = false;
	float  m_fFreeCameraSpeed = 0.05f;
	// settings
	int  KeyEnableMenu;
	int  KeyMenuExecute;
	int  KeyMenuGoBack;
	int  KeyMenuItemUP;
	int  KeyMenuItemDOWN;
	int  KeyMenuItemLEFT;
	int  KeyMenuItemRIGHT;

	int iFreeCameraKeyXPlus;
	int iFreeCameraKeyXMinus;
	int iFreeCameraKeyYPlus;
	int iFreeCameraKeyYMinus;
	int iFreeCameraKeyZPlus;
	int iFreeCameraKeyZMinus;



	void PreInitialize();
	void Initialize();

	// events

	void OnKeyLeft();
	void OnKeyRight();
	void OnKeyDown();
	void OnKeyUp();
	void OnKeyExecute();
	void OnKeyToggle();




	void ProcessMenu();
	void DrawMenu();
	void ProcessControls();

	void AddItem(std::string name, std::function<void()> func, bool toggle, int* addr, bool has_func, char* addr_sm, bool is_char, std::string model, bool swap);
	void AddFunctionEntry(std::string name, std::function<void()> func);
	void AddPlayerModelEntry(std::string name);
	void AddToggleIntEntry(std::string name, int* value);
	void AddToggleCharEntry(std::string name, char* value);
};

void HookMenu();
void WriteLine(int line, const char* string);

void PopulateSkins();

void SetSlowMotion();
void SetFastMotion();
void ResetSpeed();

void dummy();

extern eMenu TheMenu;
