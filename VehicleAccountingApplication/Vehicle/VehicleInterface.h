#pragma once

#include <sstream>
#include <limits>
#include <exception>

#include "../ExternalDependencies.h"


class VehicleInterface
{
public:
	VehicleInterface() : id_{}, type_{}, brand_{}, model_{}, production_date_{}, weight_{} {}

	VehicleInterface(size_t id) : id_{ id }, type_{}, brand_{}, model_{}, production_date_{}, weight_{} {}

	VehicleInterface(std::string id, std::string brand, std::string model, std::string date, std::string weight) : type_{}, brand_{ brand }, model_{model} {
		try
		{
			id_ = std::stoull(id);
		}
		catch (const std::exception&)
		{
			id_ = 0;
		}
		try
		{
			weight_ = std::stoul(weight);
		}
		catch (const std::exception&)
		{
			weight_ = 0;
		}

		production_date_ = date;
	}

	VehicleInterface(VehicleInterface* other) : id_{other->id_}, type_{other->type_}, brand_{other->brand_}, model_{other->model_}, production_date_{other->production_date_}, weight_{other->weight_} {};

	virtual ~VehicleInterface() {};
	virtual std::string PrintAdditions(map_Type_AdditField) = 0;
	virtual std::string ConvertToSave() {
		std::ostringstream os;
		
		os << "\"" << id_ << "\" \"";
		switch (type_)
		{
		case TypeVehicle::Car:
			os << "Car";
			break;
		case TypeVehicle::Boat:
			os << "Boat";
			break;
		case TypeVehicle::Plane:
			os << "Plane";
			break;
		case TypeVehicle::SpaceShip:
			os << "SpaceShip";
			break;
		default:
			break;
		}
		os << "\" \"" << brand_ << "\" \"" << model_ << "\" \"" << production_date_ << "\" \"" << weight_ << "\"";

		return os.str();
	}

	unsigned int GetId() const { return id_; }
	TypeVehicle GetType() const { return type_; }
	
	std::string brand_;
	std::string model_;
	Date production_date_;
	unsigned long weight_;
protected:
	TypeVehicle type_;
private:
	size_t id_;
};
