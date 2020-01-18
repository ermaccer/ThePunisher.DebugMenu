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


void PopulateSkins();
void ToggleMenu();
void ProcessMenu();
void RenderMenu();
void ControlMenu();
void AddFunction(std::string name, std::function<void()> func, bool toggle, int* addr, bool has_func, char* addr_sm, bool is_char, std::string model,bool swap);
void CallFunction(std::string name, std::function<void()> func);
void ChangePlayerModel(std::string name);
void ToggleValue(std::string name, int* value);
void ToggleValueChar(std::string name, char* value);
void HookMenu();
void WriteLine(int line, const char* string);