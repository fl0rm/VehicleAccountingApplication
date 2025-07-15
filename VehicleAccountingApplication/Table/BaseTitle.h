#pragma once

#include <iostream>
#include <string>
#include <windows.h>

#include "../ExternalDependencies.h"

class BaseTitle {
public:
	BaseTitle();

	void Animation(HANDLE hConsole, const TitlePoints& col, const ActiveWindow& visible);

	const TitleName& GetTitle() { return title_; }

	BaseTitle& operator=(const TitleName& name);
	friend std::ostream& operator << (std::ostream& os, const BaseTitle& title);

	TableSize size_;
	unsigned short width_;
private:
	TitleName title_;
};