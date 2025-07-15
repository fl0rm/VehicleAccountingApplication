
#include "VehicleCar.h"

VehicleCar::VehicleCar() : VehicleInterface(), number_of_owners_by_PTSD_{}, mileage_{} {
	type_ = TypeVehicle::Car;
}
VehicleCar::VehicleCar(unsigned int id) : VehicleInterface(id), number_of_owners_by_PTSD_{}, mileage_{} {
	type_ = TypeVehicle::Car;
}

VehicleCar::VehicleCar(std::string id, std::string brand, std::string model, std::string date, std::string weight, std::string PTSD, std::string mileage) : 
	VehicleInterface(id, brand, model, date, weight){

	type_ = TypeVehicle::Car;

	try
	{
		unsigned long l = std::stoul(PTSD);
		if (l > (std::numeric_limits<unsigned short>::max)())
			throw std::out_of_range(PTSD);

		number_of_owners_by_PTSD_ = l;
	}
	catch (const std::exception&)
	{
		number_of_owners_by_PTSD_ = 0;
	}

	try
	{
		mileage_ = std::stoull(mileage);
	}
	catch (const std::exception&)
	{
		mileage_ = 0;
	}

}

VehicleCar::VehicleCar(VehicleCar* other) : VehicleInterface(other) {
	
	this->number_of_owners_by_PTSD_ = other->number_of_owners_by_PTSD_;
	this->mileage_ = other->mileage_;
}

std::string VehicleCar::PrintAdditions(map_Type_AdditField mTA) {
	std::ostringstream os;
	os << mTA[type_][0] << ": " << number_of_owners_by_PTSD_ << ", " << mTA[type_][1] << ": " << mileage_;

	return os.str();
}

std::string VehicleCar::ConvertToSave() {
	std::ostringstream os;
	os << VehicleInterface::ConvertToSave() << " \"" << number_of_owners_by_PTSD_ << "\" \"" << mileage_ << "\"";

	return os.str();
}
