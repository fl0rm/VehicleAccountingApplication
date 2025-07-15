#pragma once

#include "VehicleInterface.h"

class VehicleBoat : public VehicleInterface {
public:
	VehicleBoat();

	VehicleBoat(unsigned int id);

	VehicleBoat(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string desplacement, std::string depth);

	VehicleBoat(VehicleBoat* other);

	std::string PrintAdditions(map_Type_AdditField mTA) override;
	std::string ConvertToSave() override;

	uint64_t desplacement_;
	uint16_t depth_screw_;
};