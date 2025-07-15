
#include "DAO.h"

DAO::DAO() {
	path_ = std::filesystem::current_path();
	name_file_ = "Без имени";
	path_to_lang_pack = "lang/";
	is_file_been_changed = false;
}

bool DAO::translit(const std::string& buf, std::vector<std::string>& vec) {
	if (buf.front() != '{' || buf.back() != '}')
		return 1;

	std::string buf_el;

	for (auto ch = buf.begin() + 1; ch != buf.end() - 1; ch++) {
		switch (*ch)
		{
		case ' ':
			if (!buf_el.empty())
				if (buf_el.front() == '\"')
					buf_el = buf_el + *ch;
			break;
		case '\"':
			if (!buf_el.empty()) {
				if (buf_el.front() == '\"') {
					buf_el.erase(buf_el.begin());
					vec.push_back(buf_el);
					buf_el.clear();
				}
			}
			else
				buf_el = buf_el + *ch;
			break;
		default:
			buf_el = buf_el + *ch;
			break;
		}
	}
	

	return 0;
}

bool DAO::translit(std::string& buf, VehicleInterface* veh) {
	buf += '{';
	
	buf += veh->ConvertToSave();

	buf += "}\n";

	return 0;
}

bool DAO::LoadLangPack(LangStruct& pack, std::string lang_) {
	std::ifstream file(path_to_lang_pack + lang_ + ".lng", std::ios::in);

	std::string buffer;
	std::vector<std::string> processed_buf;

	// Открытие файла
	if (!file.is_open()) 
		return 1;


	// Чтение названий титульника
	getline(file, buffer);
	
	if (!translit(buffer, processed_buf) && processed_buf.size() == 7)
		pack.title_.SetName(processed_buf[0], processed_buf[1], processed_buf[2], processed_buf[3], processed_buf[4], processed_buf[5], processed_buf[6]);
	else
		return 1;
	processed_buf.clear();


	// Чтение названий типов ТС
	getline(file, buffer);

	if (!translit(buffer, processed_buf) && processed_buf.size() == 4) {
		pack.vehicle_[TypeVehicle::Car] = processed_buf[0];
		pack.vehicle_[TypeVehicle::Boat] = processed_buf[1];
		pack.vehicle_[TypeVehicle::Plane] = processed_buf[2];
		pack.vehicle_[TypeVehicle::SpaceShip] = processed_buf[3];
	}
	else
		return 1;
	processed_buf.clear();


	// Чтение названий окон
	getline(file, buffer);

	if (!translit(buffer, processed_buf) && processed_buf.size() == 6) {
		pack.status_[ActiveWindow::Menu] = processed_buf[0];
		pack.status_[ActiveWindow::Table] = processed_buf[1];
		pack.status_[ActiveWindow::Save] = processed_buf[2];
		pack.status_[ActiveWindow::Open] = processed_buf[3];
		pack.status_[ActiveWindow::Select] = processed_buf[4];
		pack.status_[ActiveWindow::Edit] = processed_buf[5];
	}
	else
		return 1;
	processed_buf.clear();


	// Чтение названий окон
	getline(file, buffer);

	if (!translit(buffer, processed_buf) && processed_buf.size() == 5) {
		pack.key_[ActiveWindow::Table] = processed_buf[0][0];
		pack.key_[ActiveWindow::Edit] = processed_buf[1][0];
		pack.key_[ActiveWindow::Delete] = processed_buf[2][0];
		pack.key_[ActiveWindow::Select] = processed_buf[3][0];
		pack.key_[ActiveWindow::Sorting] = processed_buf[4][0];
	}
	else
		return 1;
	processed_buf.clear();


	// Чтение названий пунктов меню
	getline(file, buffer);

	if (!translit(buffer, processed_buf) && processed_buf.size() == 5) {
		pack.menu_.first[0] = processed_buf[0];
		pack.menu_.first[1] = processed_buf[1];
		pack.menu_.first[2] = processed_buf[4];
		pack.menu_.second[0] = processed_buf[0];
		pack.menu_.second[1] = processed_buf[1];
		pack.menu_.second[2] = processed_buf[2];
		pack.menu_.second[3] = processed_buf[3];
		pack.menu_.second[4] = processed_buf[4];
		
	}
	else
		return 1;
	processed_buf.clear();


	// Названия доп. полей

	for (int8_t i = 0; i < 4; i++) {
		std::string type;
		TypeVehicle key;
		file >> type;

		if (type == "Car") key = TypeVehicle::Car;
		else if (type == "Boat") key = TypeVehicle::Boat;
		else if (type == "Plane") key = TypeVehicle::Plane;
		else if (type == "SpaceShip") key = TypeVehicle::SpaceShip;
		else return 1;

		getline(file, buffer);

		buffer.erase(buffer.begin());
		if (!translit(buffer, processed_buf) && processed_buf.size() == pack.additionals_field_names_[key].size())
			pack.additionals_field_names_[key] = processed_buf;
		else
			return 1;

		processed_buf.clear();
	}


	// Чтение подсказок

	for (int8_t i = 0; i < 4; i++) {
		std::string type;
		ActiveWindow key;
		file >> type;

		if (type == "Table") key = ActiveWindow::Table;
		else if (type == "Edit") key = ActiveWindow::Edit;
		else if (type == "Select") key = ActiveWindow::Select;
		else if (type == "Sorting") key = ActiveWindow::Sorting;
		else return 1;

		getline(file, buffer);

		buffer.erase(buffer.begin());
		if (!translit(buffer, processed_buf) && processed_buf.size() == pack.hints_[key].size())
			pack.hints_[key] = processed_buf;
		else
			return 1;

		processed_buf.clear();
	}


	return 0;
}

std::string DAO::operator + (const std::string& st) { 
	std::string str = " - " + st;

	if (is_file_been_changed)
		str = name_file_ + '*' + str;
	else
		str = name_file_ + str;

	return str; 
}

bool DAO::WriteToFile(const vec_Vehicle& vec) {
	std::ofstream file(path_.string() + "\\" + name_file_ + ".tbl", std::ios::out);

	std::string buffer;

	// Открытие файла
	if (!file.is_open())
		return 1;

	for (auto el : vec)
		if (!translit(buffer, el)) {
			file << buffer;
			buffer.clear();
		}
		else
			return 1;
	
	is_file_been_changed = false;

	return 0;
}

bool DAO::ReadFromFile(vec_Vehicle& vec) {
	std::ifstream file(path_.string() + "\\" + name_file_ + ".tbl", std::ios::in);

	std::string buffer;
	std::vector<std::string> processed_buf;

	// Открытие файла
	if (!file.is_open())
		return 1;

	while (getline(file, buffer)) {
		if (!translit(buffer, processed_buf) && processed_buf.size() == 8) {

			TypeVehicle key;

			if (processed_buf[1] == "Car") key = TypeVehicle::Car;
			else if (processed_buf[1] == "Boat") key = TypeVehicle::Boat;
			else if (processed_buf[1] == "Plane") key = TypeVehicle::Plane;
			else if (processed_buf[1] == "SpaceShip") key = TypeVehicle::SpaceShip;
			else return 1;

			vec.push_back({});

			switch (key)
			{
			case TypeVehicle::Car:
				vec.back() = new VehicleCar(processed_buf[0], processed_buf[2], processed_buf[3], processed_buf[4], processed_buf[5], processed_buf[6], processed_buf[7]);
				break;
			case TypeVehicle::Boat:
				vec.back() = new VehicleBoat(processed_buf[0], processed_buf[2], processed_buf[3], processed_buf[4], processed_buf[5], processed_buf[6], processed_buf[7]);
				break;
			case TypeVehicle::Plane:
				vec.back() = new VehiclePlane(processed_buf[0], processed_buf[2], processed_buf[3], processed_buf[4], processed_buf[5], processed_buf[6], processed_buf[7]);
				break;
			case TypeVehicle::SpaceShip:
				vec.back() = new VehicleSpaceShip(processed_buf[0], processed_buf[2], processed_buf[3], processed_buf[4], processed_buf[5], processed_buf[6], processed_buf[7]);
				break;
			default:
				break;
			}

			processed_buf.clear();
		}
		else
			return 1;
	}

	is_file_been_changed = false;

	return 0;
}

void DAO::SetNewPath(std::string path) {
	if (std::filesystem::is_directory(path))
		path_ = path;
}

void DAO::SetNewName(std::string name_file) {
	name_file_ = name_file;
}

void DAO::GetFilesInDir(std::vector<std::string>& vec, std::string format) {
	vec.clear();

	std::filesystem::directory_iterator iter;

	try
	{
		iter = std::filesystem::directory_iterator(path_);
	}
	catch (const std::exception&)
	{
		std::cout << "File opening error";
	}

	for (const auto& entry : iter)
		if (entry.path().extension() == format)
			if (path_.string().length() == 3)
				vec.push_back(entry.path().string().substr(path_.string().length(), entry.path().string().length() - path_.string().length()));
			else
				vec.push_back(entry.path().string().substr(path_.string().length() + 1, entry.path().string().length() - path_.string().length() - 1));
}
