#pragma once

#include <vector>
#include <sstream>
#include <windows.h>

#include "TableUnitVehicle.h"
#include "ExternalDependencies.h"
#include "KeyHandler.h"

//------------------------------
//  Класс консольной графики
//------------------------------
class GCLI {
public:
	GCLI();
	~GCLI() {};
	void Start();
private:
	void ClearConsole();
	void MainMenu();
	void FrameTable();
	void SortingTable(title_points_, bool);

	std::vector<TableUnitVehicle> table_;
	std::ostringstream name_file_;
	KeyHandler input_;
	HANDLE hconsole_;
	CONSOLE_SCREEN_BUFFER_INFO csbi_;
	active_window_ visible_;
	int8_t choice_;
	bool validity_descriptor_;
	title_points_ select_point_;
	unsigned short width_, height_;
	unsigned int id_size_, brand_size_, model_size_, weight_size_, page_, select_element_;
};