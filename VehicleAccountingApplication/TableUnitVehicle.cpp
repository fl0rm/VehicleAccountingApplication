#include "TableUnitVehicle.h"

void TableUnitVehicle::SetValues(unsigned int id, type_vehicle type, std::string brand, std::string model, data production_date, unsigned int weight) {
	this->id_		= id;
	this->type_		= type;
	this->brand_	= brand;
	this->model_	= model;
	this->weight_	= weight;
	this->production_date_	= production_date;
}

void TableUnitVehicle::SetAddition() {

}