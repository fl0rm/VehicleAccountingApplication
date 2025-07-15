#include <iostream>

#include "VehicleSpaceShip.h"

VehicleSpaceShip::VehicleSpaceShip() : VehicleInterface(), used_fuel_{}, hyperjump_range_{} {
	type_ = TypeVehicle::SpaceShip;
}
VehicleSpaceShip::VehicleSpaceShip(unsigned int id) : VehicleInterface(id), used_fuel_{}, hyperjump_range_{} {
	type_ = TypeVehicle::SpaceShip;
}

VehicleSpaceShip::VehicleSpaceShip(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string fuel, std::string hyperjump) : 
	VehicleInterface(id, brand, model, date, weight), used_fuel_{} {

	type_ = TypeVehicle::SpaceShip;

	if (fuel == "KeroseneWithOxygen") used_fuel_ = TypeFuel::KeroseneWithOxygen;
	else if (fuel == "LiquidHydrogen") used_fuel_ = TypeFuel::LiquidHydrogen;
	else if (fuel == "SolidFuel") used_fuel_ = TypeFuel::SolidFuel;
	else if (fuel == "NuclearFuel") used_fuel_ = TypeFuel::NuclearFuel;
	else if (fuel == "ProtonFuel") used_fuel_ = TypeFuel::ProtonFuel;
	

	try
	{
		hyperjump_range_ = std::stoull(hyperjump);
	}
	catch (const std::exception&)
	{
		hyperjump_range_ = 0;
	}

}

VehicleSpaceShip::VehicleSpaceShip(VehicleSpaceShip* other) : VehicleInterface(other) {
	this->used_fuel_ = other->used_fuel_;
	this->hyperjump_range_ = other->hyperjump_range_;
}

std::string VehicleSpaceShip::PrintAdditions(map_Type_AdditField mTA) {
	std::ostringstream os;

	os << mTA[type_][0] << ": ";

	os << mTA[type_][static_cast<int>(used_fuel_) + 2];

	os << ", " << mTA[type_][1] << ": " << hyperjump_range_;
	
	return os.str();
}

std::string VehicleSpaceShip::ConvertToSave() {
	std::ostringstream os;
	os << VehicleInterface::ConvertToSave() << " \"";

	switch (used_fuel_)
	{
	case VehicleSpaceShip::TypeFuel::KeroseneWithOxygen:
		os << "KeroseneWithOxygen";
		break;
	case VehicleSpaceShip::TypeFuel::LiquidHydrogen:
		os << "LiquidHydrogen";
		break;
	case VehicleSpaceShip::TypeFuel::SolidFuel:
		os << "SolidFuel";
		break;
	case VehicleSpaceShip::TypeFuel::NuclearFuel:
		os << "NuclearFuel";
		break;
	case VehicleSpaceShip::TypeFuel::ProtonFuel:
		os << "ProtonFuel";
		break;
	default:
		break;
	}

	os << "\" \"" << hyperjump_range_ << "\"";

	return os.str();
}