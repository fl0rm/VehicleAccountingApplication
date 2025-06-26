#include "GCLI.h"

#include <windows.h>
#include <math.h>
#include <sstream>
#include "ExternalDependencies.h"

GCLI::GCLI() {

    table_.push_back({});
    table_[0].SetValues(10, Car, "БМВ", "M5", { 12, 10, 2021 }, 1300);
    table_.push_back({});
    table_[1].SetValues(12, Plane, "SyperJet", "205", { 30, 2, 2012 }, 19000);
    table_.push_back({});
    table_[2].SetValues(11, Car, "AUDI", "A7", { 3, 5, 2024 }, 1532);
    table_.push_back({});
    table_[3].SetValues(1432, Boat, "SpeedWagon", "S2", { 1, 1, 2020 }, 111111);
    //table_.clear();
    //table_.push_back({});
    /////////////////////////
    //------------------------------
    // Параметры консольного окна
    hconsole_ = GetStdHandle(STD_OUTPUT_HANDLE);

    validity_descriptor_ = true;

    if (hconsole_ == INVALID_HANDLE_VALUE) validity_descriptor_ = false;

    if (!GetConsoleScreenBufferInfo(hconsole_, &csbi_)) validity_descriptor_ = false;

    if (validity_descriptor_) {
        width_ = csbi_.dwSize.X;
        height_ = 10;
    }
    else {
        width_ = 0;
        height_ = 0;
    }

    page_ = 0;
    choice_ = 0;
    select_element_ = 0;
    visible_ = Menu;
    select_point_ = Id;

    //------------------------------
    // Установка отступов для колонок
    SortingTable(Id, true);
    
    id_size_ = 7;
    brand_size_ = 14;
    model_size_ = 14;
    weight_size_ = 14;


    /**
    id_size_ = log10(fabs(table_.back().GetId())) + 1;

    brand_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (brand_size_ < table_[i].GetBrand().length()) brand_size_ = table_[i].GetBrand().length();
    }

    model_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (model_size_ < table_[i].GetModel().length()) model_size_ = table_[i].GetModel().length();
    }

    weight_size_ = 0;
    for (auto i = 0; i < table_.size(); i++) {
        if (weight_size_ < table_[i].GetWeight()) weight_size_ = table_[i].GetWeight();
    }
    weight_size_ = log10(weight_size_) + 1;
    */
}


//------------------------------
// Очистка консоли 
//------------------------------
void GCLI::ClearConsole()
{
    
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };
    
    cellCount = csbi_.dwSize.X * csbi_.dwSize.Y;

    if (!FillConsoleOutputCharacter(hconsole_, ' ', cellCount, homeCoords, &count)) return;
    if (!FillConsoleOutputAttribute(hconsole_, csbi_.wAttributes, cellCount, homeCoords, &count)) return;

    SetConsoleCursorPosition(hconsole_, homeCoords);
}


//------------------------------
// Сортировка таблицы по параметрам
//------------------------------
void GCLI::SortingTable(title_points_ point, bool order) {

}


//------------------------------
// Главное меню
//------------------------------
void GCLI::MainMenu() {
    std::ostringstream os;
    std::string separation(width_, '-');
    //int8_t tmp_counter;
    std::vector<std::string> menu_points;

    os << separation << std::endl;

    if (table_.size() > 0) menu_points = { "Создать таблицу", "Открыть файл", "Редактировать таблицу", "Сохранить файл", "Вернуться", "Выйти" };
    else menu_points = { "Создать таблицу", "Открыть файл", "Выйти" };
    
    for (int8_t i = 0; i < menu_points.size(); i++) {
        if (i == choice_) os << "-> ";
        else os << "- ";
        os << menu_points[i] << std::endl;
    }
    os << separation << std::endl;

    std::cout << os.str();
}


//------------------------------
// Формирование кадра таблицы
//------------------------------
void GCLI::FrameTable() {
    std::ostringstream os;
    std::string separation(width_, '-');
    int8_t tmp_counter;
    
    switch (visible_)
    {
    case Table:
        os << "Чтение";
        break;
    case Select:
        os << "Выбор строки";
        break;
    case Edit:
        os << "Редактирование";
        break;
    default:
        break;
    }

    // Оформление титульника
    os << std::endl << separation << std::endl;
    os << "   ID\t|    Тип ТС\t|     Марка\t|    Модель\t|  Год произв.\t|      Вес\t|   Дополнительно " << std::endl;
    os << separation << std::endl;

    // Форматирование таблицы
    for (unsigned int i = page_ * height_; (i < height_ * (page_ + 1)) && (i < table_.size()); i++) {

        // Форматирование для ID
        if (table_[i].GetId() == 0) tmp_counter = 1;
        else tmp_counter = log10(fabs(table_[i].GetId())) + 1;

        if ((visible_ == Delete || visible_ == Edit) && (select_element_ == i)) { os << "->"; tmp_counter += 2; }

        for (int8_t j = 0; j < id_size_ - tmp_counter; j++) os << ' ';
        os << table_[i].GetId() << "\t|";
        
        // Форматирование типов ТС
        switch (table_[i].GetType())
        {
        case Car:
            os << "  Автомобиль\t|";
            break;
        case Boat:
            os << "     Катер\t|";
            break;
        case Plane:
            os << "    Самолет\t|";
            break;
        case SpaceShip:
            os << " Косм. корабль\r|";
            break;
        default:
            break;
        }

        // Форматирование колонки марок
        tmp_counter = table_[i].GetBrand().length();
        for (int8_t j = 0; j < brand_size_ - tmp_counter; j++) os << ' ';
        os << table_[i].GetBrand() << "\t|";

        // Форматирование колонки моделей
        tmp_counter = table_[i].GetModel().length();
        for (int8_t j = 0; j < model_size_ - tmp_counter; j++) os << ' ';
        os << table_[i].GetModel() << "\t|";

        // Форматирование колонки даты
        tmp_counter = table_[i].GetDate().length();
        for (int8_t j = 0; j < 14 - tmp_counter; j += 2) os << ' ';
        os << table_[i].GetDate() << "\t|";

        // Форматирование колонки веса
        tmp_counter = log10(table_[i].GetWeight()) + 1;
        for (int8_t j = 0; j < weight_size_ - tmp_counter; j += 2) os << ' ';
        os << table_[i].GetWeight() << "\t|";

        os << std::endl;
    }
    

    // Дорисовка остаточного места
    if ((page_ + 1) * height_ > table_.size()) for (int8_t i = table_.size() % height_; i < height_; i++) {
        
        for (int8_t j = 0; j < id_size_; j++) os << ' ';
        os << "\t|";
        for (int8_t j = 0; j < 14; j++) os << ' ';
        os << "\t|";
        for (int8_t j = 0; j < brand_size_; j++) os << ' ';
        os << "\t|";
        for (int8_t j = 0; j < model_size_; j++) os << ' ';
        os << "\t|";
        for (int8_t j = 0; j < 14; j++) os << ' ';
        os << "\t|";
        for (int8_t j = 0; j < weight_size_; j++) os << ' ';
        os << "\t|";
        os << std::endl;
    }

    os << separation << std::endl;

    // Нумирация страницы
    if (page_ > 0) {
        tmp_counter = log10(page_) - 1;
        for (int8_t j = 0; j < width_ - tmp_counter; j += 2) os << ' ';
        os << "< ";
    }
    else {
        tmp_counter = log10(page_) + 1;
        for (int8_t j = 0; j < width_ - tmp_counter; j += 2) os << ' ';
    }
    os << (page_ + 1);

    if ((page_ + 1)*height_ < table_.size()) os << " >";

    // Подсказки

    os << std::endl << separation << std::endl << "\tД - добавить  У - удалить  Ф - фильтры  С - сортировка  Р - редактировать  esc - выйти в главное меню" << std::endl << separation << std::endl;

    // Вывод содержимого в консоль
    std::cout << os.str();
}


//------------------------------
//  Отображение таблицы
//------------------------------
void GCLI::Start() {
    bool live = true;

    MainMenu();

    while (live) {

        switch (visible_)
        {
        case Menu:

            // Навигация в меню с помощью стрелок
            if ( input_.isKeyPressedOnce(VK_DOWN) && (choice_ < 5) && (choice_ < 2 || table_.size() > 0)) { choice_++; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce(VK_UP) && (choice_ > 0)) { choice_--; ClearConsole(); MainMenu(); }

            // Навигация в меню с помощью цифр
            if ( input_.isKeyPressedOnce('1')) { choice_ = 0; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('2')) { choice_ = 1; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('3')) { choice_ = 2; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('4') && (table_.size() > 0)) { choice_ = 3; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('5') && (table_.size() > 0)) { choice_ = 4; ClearConsole(); MainMenu(); }
            if ( input_.isKeyPressedOnce('6') && (table_.size() > 0)) { choice_ = 5; ClearConsole(); MainMenu(); }

            // Подтверждение выбора
            if ( input_.isKeyPressedOnce(VK_RETURN)) {

                switch (choice_)
                {
                case 0:
                    visible_ = Table;
                    ClearConsole();
                    FrameTable();
                    break;
                case 1:
                    std::cout << "Недоступно" << std::endl;
                    break;
                case 2:
                    if (table_.size() > 0) std::cout << "Недоступно" << std::endl;
                    else live = false;
                    break;
                case 3:
                    std::cout << "Недоступно" << std::endl;
                    break;
                case 4:
                    visible_ = Table;
                    ClearConsole();
                    FrameTable();
                    break;
                case 5:
                    live = false;
                    break;
                default:
                    break;
                }
            }
            break;
        case Table:

            // Переключение страниц таблицы
            if ( input_.isKeyPressedOnce(VK_LEFT) && (page_ > 0)) { page_--; ClearConsole(); FrameTable(); }
            if ( input_.isKeyPressedOnce(VK_RIGHT) && ((page_ + 1) * height_ < table_.size())) { page_++; ClearConsole(); FrameTable(); }

            // Добавление эллемента
            if (input_.isKeyPressedOnce('L')) {
                table_.push_back({}); 
                if ((page_ + 1) * height_ < table_.size())
                    page_ = (table_.size() - 1) / height_; 
                select_element_ = table_.size() - 1;

                visible_ = Edit;
                
                ClearConsole(); 
                FrameTable(); 
            }

            // Редактирование эллемента
            if (input_.isKeyPressedOnce('H')) {
                select_element_ = page_ * height_;

                select_point_ = Type;
                visible_ = Edit;

                ClearConsole();
                FrameTable();
            }

            // Удаление элемента
            if ( input_.isKeyPressedOnce('E') && table_.size() > 0) { select_element_ = page_ * height_; visible_ = Delete; ClearConsole(); FrameTable(); }

            // Выход в главное меню
            if ( input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = Menu; ClearConsole(); MainMenu(); }

            break;
        case Save:
            break;
        case Open:
            break;
        case Edit:

            // Навигация по таблице с помощью стрелок
            if (input_.isKeyPressedOnce(VK_DOWN) && (select_element_ < table_.size() - 1)) {
                if (select_element_ >= (page_ + 1) * height_) { select_element_ = 0; page_++; }
                else select_element_++;

                ClearConsole();
                FrameTable();
            }
            if (input_.isKeyPressedOnce(VK_UP) && (select_element_ > 0)) {
                if (page_ > 0) { select_element_ = height_ - 1; page_--; }
                else select_element_--;

                ClearConsole();
                FrameTable();
            }

            if (input_.isKeyPressedOnce(VK_LEFT) && ( select_point_ > Id) ) {

                select_point_ = static_cast<title_points_>(static_cast<int>(select_point_) - 1);

                ClearConsole();
                FrameTable();
            }
            if (input_.isKeyPressedOnce(VK_RIGHT) && (select_point_ < Additions)) {
                
                select_point_ = static_cast<title_points_>(static_cast<int>(select_point_) + 1);

                ClearConsole();
                FrameTable();
            }


            if (input_.isKeyPressedOnce(VK_RETURN)) {}


            // Выход из режима редактирования 
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = Table; ClearConsole(); FrameTable(); }

            break;
        case Delete:

            // Навигация по таблице с помощью стрелок
            if ( input_.isKeyPressedOnce(VK_DOWN) && (select_element_  < table_.size() - 1) ) {
                if ( select_element_ >= (page_ + 1) * height_ ) { select_element_ = 0; page_++; }
                else select_element_++;

                ClearConsole(); 
                FrameTable(); 
            }
            if (input_.isKeyPressedOnce(VK_UP) && (select_element_ > 0)) {
                if ( page_ > 0 ) { select_element_ = height_ - 1; page_--; }
                else select_element_--;

                ClearConsole(); 
                FrameTable(); 
            }

            // Подтверждение выбора
            if ( input_.isKeyPressedOnce(VK_RETURN)) {
                table_.erase(table_.begin() + select_element_);

                if (page_ * height_ + 1 > table_.size())
                    if (table_.size() == 0) page_ = 0;
                    else page_ = (table_.size() - 1) / height_;

                visible_ = Table;

                ClearConsole();
                FrameTable();
            }

            // Выход из режима удаление 
            if (input_.isKeyPressedOnce(VK_ESCAPE)) { visible_ = Table; ClearConsole(); FrameTable(); }

            break;
        default:
            break;
        }
        

        

    }

}