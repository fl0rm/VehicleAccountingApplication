#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <math.h>

#define vec_Vehicle std::vector<VehicleInterface*>
#define map_Win_Status std::map<ActiveWindow, std::string>
#define map_Win_Hints std::map<ActiveWindow, std::vector<std::string>>
#define map_Action_Key std::map<ActiveWindow, char>
#define map_Type_Name std::map<TypeVehicle, std::string>
#define map_Type_AdditField std::map<TypeVehicle, std::vector<std::string>>
#define pair_MenuPoints std::pair<std::vector<std::string>, std::vector<std::string>>

enum class ActiveWindow { Menu, Table, Save, Open, Select, Edit, Delete, Sorting};

enum class TitlePoints { Id, Type, Brand, Model, Date, Weight, Additions};

enum class TypeVehicle { Car, Boat, Plane, SpaceShip };

enum class SortOrder { Asc, Desc};

enum class VisibilityStatus { Open, Hidden };

struct Date {
	unsigned short	day_;
	unsigned short	month_;
	int	year_;

	Date() : day_{}, month_{}, year_{} {}

	Date(short day, short month, int year) {
		
		if (year > 9999) year_ = 9999;
		else if (year < 0) year_ = 0;
		else year_ = year;

		//---- 
		if (month > 12) month_ = 12;
		else if (month < 0) month_ = 0;
		else month_ = month;

		//----
		if (day < 0) day_ = 0;
		else {
			day_ = day;
			if (month_ == 2) {
				if (year_ % 4 == 0) {
					if (day > 29) day_ = 29;
				}
				else if (day > 28) day_ = 28;
			}
			else if (month_ < 8) {
				if (day > 30 + (month_ % 2)) day_ = 31;
			}
			else if (day > 31 - (month_ % 2)) day_ = 30;
		}
	}

	short length() const { 
		if (year_ == 0) return 7;
		else return 7 + log10(fabs(year_)); 
	}

	friend std::ostream& operator<<(std::ostream& os, const Date& dt){
		if (dt.day_ < 10) os << '0' << dt.day_;
		else os << dt.day_;

		if (dt.month_ < 10) os << ".0" << dt.month_;
		else os << '.' << dt.month_;

		os << '.' << dt.year_;

		return os;
	}

	Date& operator= (const std::vector<int> vec) {
		switch (vec.size())
		{
		case 0:
			return *this;
			break;
		case 1:
			if (vec[0] > 9999) this->year_ = 9999;
			else if (vec[0] < 0) this->year_ = 0;
			else this->year_ = vec[0];

			//----
			this->month_ = 1;
			this->day_ = 1;

			//----
			return *this;
			break;
		case 2:
			if (vec[1] > 9999) this->year_ = 9999;
			else if (vec[1] < 0) this->year_ = 0;
			else this->year_ = vec[1];

			//----
			if (vec[0] > 12) this->month_ = 12;
			else if (vec[0] < 0) this->month_ = 0;
			else this->month_ = vec[0];

			//----
			this->day_ = 1;

			//----
			return *this;
			break;
		case 3:
		default:
			if (vec[2] > 9999) this->year_ = 9999;
			else if (vec[2] < 0) this->year_ = 0;
			else this->year_ = vec[2];

			//----
			if (vec[1] > 12) this->month_ = 12;
			else if (vec[1] < 0) this->month_ = 0;
			else this->month_ = vec[1];

			//----
			if (vec[0] < 0) this->day_ = 0;
			else {
				this->day_ = vec[0];
				if (this->month_ == 2) {
					if (this->year_ % 4 == 0) {
						if (vec[0] > 29) this->day_ = 29;
					}
					else if (vec[0] > 28) this->day_ = 28;
				}
				else if (this->month_ < 8) {
					if (vec[0] > 30 + (this->month_ % 2)) this->day_ = 31;
				}
				else if (vec[0] > 31 - (this->month_ % 2)) this->day_ = 30;
			}

			//----
			return *this;
			break;
		}

	}

	Date& operator= (const std::string& str) {

		auto translate{ [](const std::vector<std::string>& str_vec) {
			std::vector<int> num_vec;
			size_t shift = 0;
			num_vec.resize(str_vec.size());

			for (size_t i = str_vec.size() - 1; i > 0;  i--) {
				try {
					num_vec[i] = std::stoi(str_vec[i]);
				}
				catch (const std::invalid_argument& e) {
					num_vec.erase(num_vec.begin() + i);
				}
			}

			return num_vec;
		} };


		short day{}, month{};
		int year{};
		std::vector<std::string> split_str;
		std::string tmp_str{};

		for (auto i : str) {
			if (i == ' ' || i == '.' || i == ',') { split_str.push_back(tmp_str); tmp_str.clear(); }
			else tmp_str = tmp_str + i;
		}
		split_str.push_back(tmp_str);

		for (size_t iter = split_str.size(); iter > 0; iter--)
			if (split_str[iter - 1].empty())
				split_str.erase(split_str.begin() + iter - 1);

		*this = translate(split_str);
		
		return *this;
	}

	bool operator> (const Date& dt) {

		if (this->year_ > dt.year_) return true;
		if ((this->year_ == dt.year_) && (this->month_ > dt.month_)) return true;
		if ((this->year_ == dt.year_) && (this->month_ == dt.month_) && (this->day_ > dt.day_)) return true;

		return false;
	}

	bool operator< (const Date& dt) {

		if (this->year_ < dt.year_) return true;
		if ((this->year_ == dt.year_) && (this->month_ < dt.month_)) return true;
		if ((this->year_ == dt.year_) && (this->month_ == dt.month_) && (this->day_ < dt.day_)) return true;

		return false;
	}

};

struct TitleName {
	std::map<TitlePoints, std::string> name_;

	TitleName() {
		name_[TitlePoints::Id]		= "ID";
		name_[TitlePoints::Type]	= "Type veh.";
		name_[TitlePoints::Brand]	= "Brand";
		name_[TitlePoints::Model]	= "Model";
		name_[TitlePoints::Date]	= "Date prod.";
		name_[TitlePoints::Weight] = "Weight";
		name_[TitlePoints::Additions]	= "Additionally";
	}

	TitleName(std::string id_name, std::string type_name, std::string brand_name, std::string model_name,
		std::string date_name, std::string weight_name, std::string addition_name) {
		name_[TitlePoints::Id]		= id_name;
		name_[TitlePoints::Type]	= type_name;
		name_[TitlePoints::Brand]	= brand_name;
		name_[TitlePoints::Model]	= model_name;
		name_[TitlePoints::Date]	= date_name;
		name_[TitlePoints::Weight] = weight_name;
		name_[TitlePoints::Additions]	= addition_name;
	}

	TitleName(const TitleName& name) {
		name_[TitlePoints::Id]		= name.name_.at(TitlePoints::Id);
		name_[TitlePoints::Type]	= name.name_.at(TitlePoints::Type);
		name_[TitlePoints::Model]	= name.name_.at(TitlePoints::Model);
		name_[TitlePoints::Date]	= name.name_.at(TitlePoints::Date);
		name_[TitlePoints::Weight]	= name.name_.at(TitlePoints::Weight);
		name_[TitlePoints::Additions]	= name.name_.at(TitlePoints::Additions);
	}

	void SetName(std::string id_name, std::string type_name, std::string brand_name, std::string model_name,
		std::string date_name, std::string weight_name, std::string addition_name) {
		name_[TitlePoints::Id]		= id_name;
		name_[TitlePoints::Type]	= type_name;
		name_[TitlePoints::Brand]	= brand_name;
		name_[TitlePoints::Model]	= model_name;
		name_[TitlePoints::Date]	= date_name;
		name_[TitlePoints::Weight]	= weight_name;
		name_[TitlePoints::Additions]	= addition_name;
	}

	TitleName& operator= (const TitleName& name) {
		if (this == &name) return *this;

		this->name_[TitlePoints::Id]		= name.name_.at(TitlePoints::Id);
		this->name_[TitlePoints::Type]		= name.name_.at(TitlePoints::Type);
		this->name_[TitlePoints::Brand]		= name.name_.at(TitlePoints::Brand);
		this->name_[TitlePoints::Model]		= name.name_.at(TitlePoints::Model);
		this->name_[TitlePoints::Date]		= name.name_.at(TitlePoints::Date);
		this->name_[TitlePoints::Weight]	= name.name_.at(TitlePoints::Weight);
		this->name_[TitlePoints::Additions]	= name.name_.at(TitlePoints::Additions);

		return *this;
	}

};

struct TableSize {
	std::map<TitlePoints, unsigned short> size_;

	TableSize() {};

	TableSize(const TitleName& title) {
		size_[TitlePoints::Id]		= title.name_.at(TitlePoints::Id).length();
		size_[TitlePoints::Type]	= title.name_.at(TitlePoints::Type).length();
		size_[TitlePoints::Brand]	= title.name_.at(TitlePoints::Brand).length();
		size_[TitlePoints::Model]	= title.name_.at(TitlePoints::Model).length();
		size_[TitlePoints::Date]	= title.name_.at(TitlePoints::Date).length();
		size_[TitlePoints::Weight]	= title.name_.at(TitlePoints::Weight).length();
		size_[TitlePoints::Additions]	= title.name_.at(TitlePoints::Additions).length();
	}

	TableSize(const TableSize& size) {
		size_[TitlePoints::Id]		= size.size_.at(TitlePoints::Id);
		size_[TitlePoints::Type]	= size.size_.at(TitlePoints::Type);
		size_[TitlePoints::Brand]	= size.size_.at(TitlePoints::Brand);
		size_[TitlePoints::Model]	= size.size_.at(TitlePoints::Model);
		size_[TitlePoints::Date]	= size.size_.at(TitlePoints::Date);
		size_[TitlePoints::Weight]	= size.size_.at(TitlePoints::Weight);
		size_[TitlePoints::Additions]	= size.size_.at(TitlePoints::Additions);
	}

	TableSize& operator= (const TitleName& title) {
		this->size_[TitlePoints::Id]		= title.name_.at(TitlePoints::Id).length();
		this->size_[TitlePoints::Type]		= title.name_.at(TitlePoints::Type).length();
		this->size_[TitlePoints::Brand]		= title.name_.at(TitlePoints::Brand).length();
		this->size_[TitlePoints::Model]		= title.name_.at(TitlePoints::Model).length();
		this->size_[TitlePoints::Date]		= title.name_.at(TitlePoints::Date).length();
		this->size_[TitlePoints::Weight]	= title.name_.at(TitlePoints::Weight).length();
		this->size_[TitlePoints::Additions]	= title.name_.at(TitlePoints::Additions).length();

		return *this;
	}

	TableSize& operator= (const TableSize& size) {

		if (this == &size) return *this;

		this->size_[TitlePoints::Id]	= size.size_.at(TitlePoints::Id);
		this->size_[TitlePoints::Type]	= size.size_.at(TitlePoints::Type);
		this->size_[TitlePoints::Brand] = size.size_.at(TitlePoints::Brand);
		this->size_[TitlePoints::Model] = size.size_.at(TitlePoints::Model);
		this->size_[TitlePoints::Date]	= size.size_.at(TitlePoints::Date);
		this->size_[TitlePoints::Weight]	= size.size_.at(TitlePoints::Weight);
		this->size_[TitlePoints::Additions] = size.size_.at(TitlePoints::Additions);

		return *this;
	}

	unsigned short sym() const { return size_.at(TitlePoints::Id) + size_.at(TitlePoints::Type) 
		+ size_.at(TitlePoints::Brand) + size_.at(TitlePoints::Model) + size_.at(TitlePoints::Date) 
		+ size_.at(TitlePoints::Weight) + size_.at(TitlePoints::Additions) + 6; }

};

struct LangStruct {
	TitleName title_;
	map_Win_Status status_;
	map_Win_Hints hints_;
	map_Action_Key key_;
	map_Type_Name vehicle_;
	map_Type_AdditField additionals_field_names_;
	pair_MenuPoints menu_;

	LangStruct() : title_{}, vehicle_{} {
		additionals_field_names_[TypeVehicle::Car].resize(2);
		additionals_field_names_[TypeVehicle::Boat].resize(2);
		additionals_field_names_[TypeVehicle::Plane].resize(2);
		additionals_field_names_[TypeVehicle::SpaceShip].resize(7);
		menu_.first.resize(3);
		menu_.second.resize(5);
		hints_[ActiveWindow::Table].resize(4);
		hints_[ActiveWindow::Edit].resize(2);
		hints_[ActiveWindow::Select].resize(4);
		hints_[ActiveWindow::Sorting].resize(2);
	}
};
