#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>

#include "DAO.h"
#include "../ExternalDependencies.h"

class FileProcessorInterface {
public:
	FileProcessorInterface();

	void SetSize(uint16_t width, uint16_t row_count);
	void SetTitle(ActiveWindow vis);
	bool LoadLangPack(LangStruct& pack);

	void RenderingFileSelector();

	void MoveCursorUp();
	void MoveCursorDown();

	void NextPage();
	void PreviosPage();

	bool SelectReadPosition(vec_Vehicle& vec, HANDLE hConsole);
	bool SelectWritePosition(const vec_Vehicle& vec, HANDLE hConsole);

	void FileChanged() { file_processor_.FileChanged(); }

private:
	DAO file_processor_;
	std::vector<std::string> all_dirs_;
	std::vector<std::string> all_files_;
	map_Win_Status status_;
	int64_t choice_;
	uint16_t width_, row_count_;
	uint32_t page_;
};