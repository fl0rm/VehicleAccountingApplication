#pragma once

#include "VehicleInterface.h"

class VehicleCar : public VehicleInterface {
public:
	VehicleCar();

	VehicleCar(unsigned int id);

	VehicleCar(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string PTSD, std::string mileage);

	VehicleCar(VehicleCar* other);

	std::string PrintAdditions(map_Type_AdditField mTA) override;
	std::string ConvertToSave() override;
	
	uint16_t number_of_owners_by_PTSD_;
	uint64_t mileage_;
};