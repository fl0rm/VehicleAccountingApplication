#pragma once

#include "VehicleInterface.h"

class VehiclePlane : public VehicleInterface {
public:
	VehiclePlane();

	VehiclePlane(unsigned int id);

	VehiclePlane(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string load_cap, std::string wingspan);

	VehiclePlane(VehiclePlane* other);

	std::string PrintAdditions(map_Type_AdditField mTA) override;
	std::string ConvertToSave() override;

	uint64_t load_capacity_;
	uint64_t wingspan_;
};