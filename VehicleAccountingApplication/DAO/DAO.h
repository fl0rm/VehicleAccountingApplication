#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#include "../ExternalDependencies.h"
#include "../Vehicle/VehicleInterface.h"
#include "../Vehicle/VehicleCar.h"
#include "../Vehicle/VehicleBoat.h"
#include "../Vehicle/VehiclePlane.h"
#include "../Vehicle/VehicleSpaceShip.h"

class DAO {
public:
	DAO();

	void SetNewPath(std::string path);
	void SetNewName(std::string name_file);
	std::string GetPath() const { return path_.string(); }
	std::string GetName() const { return name_file_; }
	void GetFilesInDir(std::vector<std::string>& vec, std::string format = "");

	bool WriteToFile(const vec_Vehicle& vec);

	bool LoadLangPack(LangStruct& pack, std::string lang_ = "RU");
	bool ReadFromFile(vec_Vehicle& vec);

	void FileChanged() { is_file_been_changed = true; }
	std::string operator + (const std::string& st);
private:
	bool translit(const std::string& buf, std::vector<std::string>& vec);
	bool translit(std::string& buf, VehicleInterface* veh);

	std::filesystem::path path_;
	std::string name_file_;
	std::string path_to_lang_pack;
	bool is_file_been_changed;
};