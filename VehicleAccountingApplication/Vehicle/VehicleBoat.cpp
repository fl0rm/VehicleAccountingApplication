
#include "VehicleBoat.h"

VehicleBoat::VehicleBoat() : VehicleInterface(), desplacement_{}, depth_screw_{} {
	type_ = TypeVehicle::Boat;
}
VehicleBoat::VehicleBoat(unsigned int id) : VehicleInterface(id), desplacement_{}, depth_screw_{} {
	type_ = TypeVehicle::Boat;
}

VehicleBoat::VehicleBoat(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string desplacement, std::string depth) : 
	VehicleInterface(id, brand, model, date, weight) {

	type_ = TypeVehicle::Boat;

	try
	{
		desplacement_ = std::stoull(desplacement);
	}
	catch (const std::exception&)
	{
		desplacement_ = 0;
	}

	try
	{
		unsigned long l = std::stoul(depth);
		if (l > (std::numeric_limits<unsigned short>::max)())
			throw std::out_of_range(depth);
		
		depth_screw_ = l;
	}
	catch (const std::exception&)
	{
		depth_screw_ = 0;
	}

}

VehicleBoat::VehicleBoat(VehicleBoat* other) : VehicleInterface(other) {
	this->desplacement_ = other->desplacement_;
	this->depth_screw_ = other->depth_screw_;
}

std::string VehicleBoat::PrintAdditions(map_Type_AdditField mTA) {
	std::ostringstream os;
	
	os << mTA[type_][0] << ": " << desplacement_ << ", " << mTA[type_][1] << ": " << depth_screw_;

	return os.str();
}

std::string VehicleBoat::ConvertToSave() {
	std::ostringstream os;
	os << VehicleInterface::ConvertToSave() << " \"" << desplacement_ << "\" \"" << depth_screw_ << "\"";

	return os.str();
}