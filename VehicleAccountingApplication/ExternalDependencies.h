#pragma once
#include <iostream>
#include <math.h>

enum active_window_ { Menu, Table, Save, Open, Select, Edit, Delete};

enum title_points_ { Id, Type, Brand, Model, Date, Weight, Additions};

enum type_vehicle { Car, Boat, Plane, SpaceShip };

struct data {
	unsigned short	day_;
	unsigned short	month_;
	int	year_;

	data() : day_{}, month_{}, year_{} {}
	data(short day, short month, int year) {
		
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

	friend std::ostream& operator<<(std::ostream& os, const data& dt){
		if (dt.day_ < 10) os << '0' << dt.day_;
		else os << dt.day_;

		if (dt.month_ < 10) os << ".0" << dt.month_;
		else os << '.' << dt.month_;

		os << '.' << dt.year_;

		return os;
	}
};

struct additional_field {
	unsigned short	id_;
	std::string		field_;
	additional_field() : id_{0}, field_{""} {}
};