#pragma once

#include "VehicleInterface.h"

class VehicleSpaceShip : public VehicleInterface {
private:
	enum class TypeFuel { KeroseneWithOxygen, LiquidHydrogen, SolidFuel, NuclearFuel, ProtonFuel };
public:
	VehicleSpaceShip();

	VehicleSpaceShip(unsigned int id);

	VehicleSpaceShip(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string fuel, std::string hyperjump);

	VehicleSpaceShip(VehicleSpaceShip* other);

	std::string PrintAdditions(map_Type_AdditField mTA) override;
	std::string ConvertToSave() override;
	
	TypeFuel used_fuel_;
	uint64_t hyperjump_range_;
};