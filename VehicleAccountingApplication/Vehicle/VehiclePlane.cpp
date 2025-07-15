
#include "VehiclePlane.h"

VehiclePlane::VehiclePlane() : VehicleInterface(), load_capacity_{}, wingspan_{} {
	type_ = TypeVehicle::Plane;
}
VehiclePlane::VehiclePlane(unsigned int id) : VehicleInterface(id), load_capacity_{}, wingspan_{} {
	type_ = TypeVehicle::Plane;
}

VehiclePlane::VehiclePlane(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string load_cap, std::string wingspan) : 
	VehicleInterface(id, brand, model, date, weight) {

	type_ = TypeVehicle::Plane;

	try
	{
		load_capacity_ = std::stoull(load_cap);
	}
	catch (const std::exception&)
	{
		load_capacity_ = 0;
	}

	try
	{
		wingspan_ = std::stoull(wingspan);
	}
	catch (const std::exception&)
	{
		wingspan_ = 0;
	}

}

VehiclePlane::VehiclePlane(VehiclePlane* other) : VehicleInterface(other) {
	this->load_capacity_ = other->load_capacity_;
	this->wingspan_ = other->wingspan_;
}

std::string VehiclePlane::PrintAdditions(map_Type_AdditField mTA) {
	std::ostringstream os;
	
	os << mTA[type_][0] << ": " << load_capacity_ << ", " << mTA[type_][1] << ": " << wingspan_;

	return os.str();
}

std::string VehiclePlane::ConvertToSave() {
	std::ostringstream os;
	os << VehicleInterface::ConvertToSave() << " \"" << load_capacity_ << "\" \"" << wingspan_ << "\"";

	return os.str();
}