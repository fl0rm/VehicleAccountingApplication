
#include "BaseTitle.h"

#include <iostream>
#include <chrono>

//------------------------------
// Конструктор
//------------------------------
BaseTitle::BaseTitle() {
	size_ = title_;
	width_ = 120;
}


//------------------------------
// Установка нового титульника
//------------------------------
BaseTitle& BaseTitle::operator=(const TitleName& tl) {
	this->title_ = tl;
	this->size_ = this->title_;

	return *this;
}


//------------------------------
// Анимация выбора столбца
//------------------------------
void BaseTitle::Animation(HANDLE hConsole, const TitlePoints& col, const ActiveWindow& visible) {
	static VisibilityStatus status = VisibilityStatus::Open;

	// Работа со временем 
	static auto check_point_time = std::chrono::steady_clock::now();
	auto time_now = std::chrono::steady_clock::now();

	std::chrono::duration<int, std::centi> required_interval{ 55 }; // Задержка 0,55 сек. Период анимации 1,1 сек.
	std::chrono::duration<double, std::centi> received_interval = time_now - check_point_time;


	if (received_interval >= required_interval) {
		
		// Установка отступов
		unsigned short offset = (width_ - size_.sym()) / 7;
		short overall_offset = offset / 2;
		for (int8_t i = 0; i < static_cast<int>(col); i++) overall_offset += offset + size_.size_.at(static_cast<TitlePoints>(i)) + 1;

		if ((col > TitlePoints::Type && visible == ActiveWindow::Edit) ||
			(col > TitlePoints::Id && visible == ActiveWindow::Sorting)) overall_offset -= 2;

		// Подготовка названия столбца
		std::string column = title_.name_.at(col);

		switch (col)
		{
		case TitlePoints::Id:
			column = column + " >";
			break;
		case TitlePoints::Type:
			if (visible == ActiveWindow::Edit) column = column + " >";
			if (visible == ActiveWindow::Sorting) column = "< " + column + " >";
			break;
		case TitlePoints::Weight:
			if (visible == ActiveWindow::Edit) column = "< " + column + " >";
			if (visible == ActiveWindow::Sorting) column = "< " + column;
			break;
		case TitlePoints::Additions:
			column = "< " + column;
			break;
		default:
			column = "< " + column + " >";
			break;
		}


		// Позиционирование курсора
		DWORD count;
		COORD coord;
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(hConsole, &csbi);
		coord = csbi.dwCursorPosition;

		// Анимация
		switch (status)
		{
		case VisibilityStatus::Open:
			FillConsoleOutputCharacterA(hConsole, ' ', column.length(), {overall_offset, 1}, &count);


			check_point_time = std::chrono::steady_clock::now();
			status = VisibilityStatus::Hidden;
			break;
		case VisibilityStatus::Hidden:
			SetConsoleCursorPosition(hConsole, {overall_offset, 1});
			WriteConsoleA(hConsole, column.c_str(), column.length(), &count, NULL);

			check_point_time = std::chrono::steady_clock::now();
			status = VisibilityStatus::Open;
			break;
		default:
			break;
		}

		SetConsoleCursorPosition(hConsole, coord);
	}

}


//------------------------------
// Форматирование вывода титульника
//------------------------------
std::ostream& operator << (std::ostream& os, const BaseTitle& tl) {
	unsigned short offset = (tl.width_ - tl.size_.sym()) / 7;
	unsigned short shift;

	for (size_t i = 0; i < 7; i++) {
		shift = offset + tl.size_.size_.at(static_cast<TitlePoints>(i)) - tl.title_.name_.at(static_cast<TitlePoints>(i)).length();
		for (size_t j = 0; j < offset / 2; j++) os << ' ';
		os << tl.title_.name_.at(static_cast<TitlePoints>(i));
		for (size_t j = 0; j < offset / 2; j++) os << ' ';
		if (offset % 2 == 1) os << ' ';
		if (i < 6) os << '|';
	}

	return os;
}