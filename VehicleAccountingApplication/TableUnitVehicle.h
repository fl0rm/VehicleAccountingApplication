#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "ExternalDependencies.h"


//------------------------------
//  Класс одной единицы таблицы
//------------------------------
class TableUnitVehicle {
public:
	TableUnitVehicle() : id_{}, type_{}, brand_{}, model_{}, weight_{}, production_date_{} {}
	void SetValues(unsigned int, type_vehicle, std::string, std::string, data, unsigned int);
	void SetAddition();
	unsigned int GetId() const { return id_; }
	type_vehicle GetType() const { return type_; }
	std::string GetBrand() const { return brand_; }
	std::string GetModel() const { return model_; }
	data GetDate() const { return production_date_; }
	unsigned int GetWeight() const { return weight_; }
private:
	unsigned int id_;
	type_vehicle type_;
	std::string brand_;
	std::string model_;
	data production_date_;
	unsigned int weight_;
	std::vector<additional_field> addition_;
};