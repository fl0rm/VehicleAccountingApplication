#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <functional>
#include <stdexcept>

#include "BaseTitle.h"
#include "../KeyHandler.h"
#include "../ExternalDependencies.h"
#include "../Vehicle/VehicleInterface.h"
#include "../Vehicle/VehicleCar.h"
#include "../Vehicle/VehicleBoat.h"
#include "../Vehicle/VehiclePlane.h"
#include "../Vehicle/VehicleSpaceShip.h"

class BaseTable {
public:
	BaseTable();
	~BaseTable();

	void RenderingTheTable( ActiveWindow visible_);
	void RenderingTheLine();
	void SelectionAnimation( HANDLE hConsole, const ActiveWindow& visible);

	void SetTable(const vec_Vehicle& tb);
	void SetColumn();
	void SetCursor();
	void SetLang(const LangStruct& pack);

	void SortingTable();

	void NextPage();
	void PreviosPage();

	void NextColumn(const ActiveWindow& visible);
	void PreviosColumn(const ActiveWindow& visible);

	void SelectElemUp();
	void SelectElemDown();

	void AddElement();
	void EditElement(HANDLE hConsole ,KeyHandler& input);
	void DeleteElement();

	TitlePoints GetPoint() const { return select_point_; }
	const vec_Vehicle& GetTable() { return pure_table_; }

	size_t size_of_pure_table() { return pure_table_.size(); }
	size_t size_of_conditions_table() { return conditions_table_.size(); }
	void clear();

private:
	std::ostringstream RederingTheCell(unsigned int i, TitlePoints cell, ActiveWindow visible_);
	TypeVehicle ChangeType(HANDLE hConsole ,KeyHandler input, COORD coord);
	void Rebalance();

	std::vector <std::pair< VehicleInterface*, size_t >> conditions_table_;
	vec_Vehicle pure_table_;
	map_Type_Name vehicle_;
	map_Type_AdditField additionals_field_names_;
	map_Win_Hints hints_;
	BaseTitle title_;
	TitlePoints select_point_;
	TableSize column_;
	SortOrder order_;
	unsigned int select_element_;
	unsigned int page_;
	unsigned short width_;
	unsigned short row_count_;

};