#pragma once

#include <vector>
#include <sstream>
#include <windows.h>

#include "../ExternalDependencies.h"
#include "../Table/BaseTable.h"
#include "../DAO/FileProcessorInterface.h"
#include "../KeyHandler.h"

//------------------------------
//  Класс консольной графики
//------------------------------
class GCLI {
public:
	GCLI();
	void Start();
private:
	void ClearConsole() const;
	void MainMenu();
	
	BaseTable table_;
	FileProcessorInterface file_processor_;
	KeyHandler input_;
	HANDLE hconsole_;
	CONSOLE_SCREEN_BUFFER_INFO csbi_;
	ActiveWindow visible_;
	LangStruct lang_pack_;
	map_Action_Key key_;
	pair_MenuPoints menu_points_;
	int choice_;
	bool is_validity_descriptor_;
	unsigned short width_, height_;
};